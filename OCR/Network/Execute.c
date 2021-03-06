//
//  Execute.c
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include "Execute.h"

#ifndef DEBUG
#define DEBUG 0
#endif

void teach(Network *net, DataSource data[], size_t dataLen, double errorCoeff, double learningCoeff) {
    printf("Teaching process started\n");
    double curentErr = 1;
    int count = 0;
    while (curentErr > errorCoeff) {
        count++;
        curentErr = teachData(net, data[0], learningCoeff);
        for (size_t i = 1; i < dataLen; i++) {
            curentErr += teachData(net, data[i], learningCoeff);
        }
        curentErr /= 4;
        
#if DEBUG > 1
        if (count % 10 == 0) {
            printf("Itération : %i\n    Current Error: %.4f\n", count, curentErr);
        }
#endif
        
        if (count > 500000 && count % 500000 == 0) {
            warnx("The iteration number is unusually high: %i\nCurrent Error: %f\n",count, curentErr);
        }
    }
    printf("Teaching process completed\n   Current Error: %f\n   Number of iterations: %i\n", curentErr, count);
}

void compute(Network *net, DataSource data) {
    for (int neuron = 0; neuron < net->enters; neuron++)
        net->A[neuron] = data.enters[neuron];
    for (int neuron = net->enters; neuron < net->totalH; neuron++) {
        updateIn(net, neuron);
        net->A[neuron] = sigmoide(net->In[neuron]);
    }
}

double teachData(Network *net, DataSource data, double alpha) {
    int begin, end, begin2, end2;
    compute(net, data);
    
    double error = 0;
    getNeurons(*net, net->hLayers + 1, &begin, &end);
    for (int n = begin; n <= end; n++) {
        int pos = nbOfNeuroneInLayer(*net, n);
        error += fabs(net->A[n] - data.out[pos]);//data.response;
    }
    error /= net->out;
    
    getNeurons(*net, net->hLayers + 1, &begin, &end);
    for (int n = begin; n <= end; n++) {
        int pos = nbOfNeuroneInLayer(*net, n);
        net->D[n] = (data.out[pos] - net->A[n]);
    }
    for (int neuron = net->totalH - net->out - 1; neuron >= net->enters; neuron--)
        updateDelta(net, neuron, 0, 0);
    
    for (int layer = 0; layer < net->hLayers + 1; layer++) {
        getNeurons(*net, layer, &begin, &end);
        getNeurons(*net, layer + 1, &begin2, &end2);
        for (int n1 = begin; n1 <= end; n1++) {
            for (int n2 = begin2; n2 <= end2; n2++) {
                double w = getWeight(*net, n1, n2);
                w += net->A[n1] * net->D[n2] * alpha;
                setWeight(net, n1, n2, w);
            }
        }
    }
    return error;
}

char get_result(Network net) {
    int begin, end, buff_index;
    getNeurons(net, net.hLayers + 1, &begin, &end);
    buff_index = begin;
    for (int i = begin + 1; i <= end; i++) {
        if (net.A[buff_index] < net.A[i])
            buff_index = i;
    }
    return (char)(buff_index - begin + 'A');
}
