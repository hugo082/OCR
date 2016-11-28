//
//  Core.c
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//
//  Pour la description des fonctions :
//  voir le fichier Core.h
//

#include "Core.h"
#include <math.h>

/*
 Retoune un random double entre deux bornes.
 **/
double drand(double a, double b){
    return (rand() / (double)RAND_MAX) * (b-a) + a;
}

Network *network_new(int enters, int out, int hLayers, int nByLayer) {
    Network *net = malloc(sizeof(Network));
    net->enters = enters;
    net->out = out;
    net->hLayers = hLayers;
    net->nByLayer = nByLayer;
    int h = enters + out + (hLayers * nByLayer);
    net->totalH = h;
    
    int nbWeight =  enters * nByLayer + out * nByLayer + (hLayers - 1) * (nByLayer * nByLayer);
    net->weight = malloc(sizeof(double) * nbWeight);
    net->A = malloc(sizeof(double) * h);
    net->In = malloc(sizeof(double) * h);
    net->D = malloc(sizeof(double) * h);
    
    init_weight(net, nbWeight);
    
    return net;
}

void init_weight(Network *net, int nbWeight) {
    for (int i = 0; i < nbWeight; i++) {
        net->weight[i] = drand(-1, 1);
    }
}

int weightIndex(Network net, int n1, int n2) {
    int layer1 = getLayer(net, n1);
    int layer2 = getLayer(net, n2);
    
    if(layer1 != (layer2 - 1) || layer1 < 0) {
        warnx("Access to invalid weight: %i to %i", n1, n2);
        return -1;
    }
    
    int weightBefL1 = 0;
    if (layer1 > 0) {
        int carre = net.nByLayer * net.nByLayer;
        for (int layer = (layer1 - 1); layer >= 0; layer--) {
            if (layer == 0) {
                weightBefL1 += net.enters * net.nByLayer;
            } else {
                weightBefL1 += carre;
            }
        }
    }
    int pos1 = nbOfNeuroneInLayer(net, n1);
    int pos2 = nbOfNeuroneInLayer(net, n2);
    
    int offset = 0;
    if (layer2 == net.hLayers + 1) {
        offset -= (net.nByLayer - net.out) * pos1;
    }
    return weightBefL1 + pos1 * net.nByLayer + pos2 + offset;
}

double getWeight(Network net, int n1, int toN2) {
    int index = weightIndex(net, n1, toN2);
    if (index != -1) {
        return net.weight[index];
    }
    errx(20, "Invalid index for getWeight(net:, n1:, toN2)");
}

void setWeight(Network *net, int n1, int toN2, double value) {
    int index = weightIndex(*net, n1, toN2);
    if (index != -1) {
        net->weight[index] = value;
    } else {
        errx(21, "Invalid index for getWeight(net:, n1:, toN2)");
    }
}

int getLayer(Network net, int neuron){
    if (neuron < 0) {
        errx(22, "Search layer of invalid neuron: %i", neuron);
    }else if (neuron < net.enters) {
        return 0;
    }
    return ((neuron - net.enters) / net.nByLayer) + 1;
}

int nbOfNeuroneInLayer(Network net, int n) {
    int layer = getLayer(net, n);
    if (layer < 1) {
        return n;
    }
    return n - ((layer - 1) * net.nByLayer) - net.enters;
}

int iemeNeuronInLayer(Network net, int neuron, int layer) {
    if(layer == 0) {
        return neuron;
    }
    return (layer - 1) * net.nByLayer + net.enters + neuron;
}

void getNeurons(Network net, int layer, int *begin, int *end) {
    if (layer == 0) {
        *begin = 0;
        *end = net.enters - 1;
    } else if (layer == net.hLayers + 1) {
        *begin = net.enters + net.hLayers * net.nByLayer;
        *end = net.totalH - 1;
    } else {
        int start = net.enters + (layer - 1) * net.nByLayer;
        *begin = start;
        *end = start + net.nByLayer - 1;
    }
}

void updateIn(Network *net, int neuron) {
    int begin, end;
    int layer = getLayer(*net, neuron);
    getNeurons(*net, layer - 1, &begin, &end);
    
    double in = 0;
    for (int i = begin; i <= end; i++) {
        in += net->A[i] * getWeight(*net, i, neuron);
    }
    net->In[neuron] = in;
}

void updateDelta(Network *net, int neuron, double y, double h) {
    int begin, end;
    int layer = getLayer(*net, neuron);
    if (layer == net->hLayers + 1) {
        net->D[neuron] = y - h;
        return;
    }
    getNeurons(*net, layer + 1, &begin, &end);
    
    double wSum = 0;
    for (int i = begin; i <= end; i++) {
        double w = getWeight(*net, neuron, i);
        wSum += w * net->D[i];
    }
    net->D[neuron] = sigmoidePrime(net->A[neuron]) * wSum;
}

double sigmoide(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoidePrime(double x) {
    return x * (1.0 - x);
}
