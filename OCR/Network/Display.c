//
//  Display.c
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//
//  Pour la description des fonctions :
//  voir le fichier Display.h
//

#include "Display.h"

void sortedReplace(Network net, int *array, int size, int value) {
    int buff;
    for (int i = 0; i < size; i++) {
        if (net.A[value] > net.A[array[i]]) {
            buff = array[i];
            array[i] = value;
            value = buff;
        }
    }
}

void sortedInsert(Network net, int *array, int *size, int value) {
    int buff;
    for (int i = 0; i < *size; i++) {
        if (net.A[value] > net.A[array[i]]) {
            buff = array[i];
            array[i] = value;
            value = buff;
        }
    }
    array[*size] = value;
    *size += 1;
}

void printBestOutput(Network net, int count) {
    int begin, end;
    printf("Result of Network:\n");
    int *best = malloc(sizeof(int) * count);
    int index = 0;
    getNeurons(net, net.hLayers + 1, &begin, &end);
    for (int i = begin; i <= end; i++) {
        if (index < count) {
            sortedInsert(net, best, &index, i);
        } else {
            sortedReplace(net, best, index, i);
        }
    }
    for (int i = 0; i < index; i++) {
        int z = best[i];
        printf("   Output %i (%i) (%c): %f\n", z - begin, z, (char)(z - begin + 'A'), net.A[z]);
    }
}

void printTheoricalResult(Network net, DataSource d) {
    int begin, end, buff_index;
    printf("Result of Network (%s):\n", d.identifier);
    getNeurons(net, net.hLayers + 1, &begin, &end);
    buff_index = begin;
    for (int i = begin + 1; i <= end; i++) {
        if (net.A[buff_index] < net.A[i])
            buff_index = i;
    }
    printf("   Output %i (%i) (%c): %f\n", buff_index - begin, buff_index, (char)(buff_index - begin + 'A'), net.A[buff_index]);
    
}

void printResult(Network net) {
    int begin, end;
    printf("Result of Network:\n");
    getNeurons(net, 0, &begin, &end);
//    for (int i = begin; i <= end; i++) {
//        printf("   Enter %i : %f\n", i, net.A[i]);
//    }
    getNeurons(net, net.hLayers + 1, &begin, &end);
    for (int i = begin; i <= end; i++) {
        printf("   Output %i (%i) (%c): %f\n", i - begin, i, (char)(i - begin + 'A'), net.A[i]);
    }
}

void printValue(Network net) {
    
    for (int n = 0; n < net.nByLayer; n++) {
        for (int l = 0; l < net.hLayers + 2; l++) {
            int index = n;
            
            if (l == 0) {
                if (n < net.enters)
                    printf("%i:%.2f  -  ", index, net.A[index]);
                else
                    printf("        -  ");
            }else if (l == net.hLayers + 1) {
                index += net.enters;
                index += net.nByLayer * (l - 1);
                if (n < net.out)
                    printf("%i:%.2f  -  ", index, net.A[index]);
                
            }else {
                index += net.enters;
                index += net.nByLayer * (l - 1);
                printf("%i:%.2f  -  ", index, net.A[index]);
            }
            
        }
        printf("\n\n");
    }
}

void printDeltas(Network net) {
    
    for (int n = 0; n < net.nByLayer; n++) {
        for (int l = 0; l < net.hLayers + 2; l++) {
            int index = n;
            
            if (l == 0) {
                if (n < net.enters)
                    printf("D:%.2f  -  ", net.D[index]);
                else
                    printf("        -  ");
            }else if (l == net.hLayers + 1) {
                index += net.enters;
                index += net.nByLayer * (l - 1);
                if (n < net.out)
                    printf("D:%.2f  -  ", net.D[index]);
                
            }else {
                index += net.enters;
                index += net.nByLayer * (l - 1);
                printf("D:%.2f  -  ", net.D[index]);
            }
            
        }
        printf("\n\n");
    }
}

void printWeight(Network net) {
    
    int begin1, end1, begin2, end2;
    for (int layer = 0; layer < net.hLayers + 1; layer++) {
        getNeurons(net, layer, &begin1, &end1);
        getNeurons(net, layer, &begin2, &end2);
        for (int n1 = begin1; n1 <= end1; n1++) {
            for (int n2 = begin2; n2 <= end2; n2++) {
                double w = getWeight(net, n1, n2);
                printf("N:%i to N:%i => %f\n", n1, n2, w);
            }
        }
    }
}
