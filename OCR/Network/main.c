//
//  main.c
//  EpitaOCR
//
//  Created by Hugo on 05/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include <stdlib.h>

#include "Save.h"
#include "Core.h"
#include "Execute.h"
#include "Display.h"

char* getLine(int maxSize) {
    int i = 0, c = 0;
    char *buffer = malloc(sizeof(char) * maxSize);
    while ((c = getchar()) != 10 && i < maxSize)
        *(buffer + i++) = c;
    return buffer;
}

int main() {
    
    double errCoef;
    do {
        printf("Max Error : ");
        char *err = getLine(10);
        errCoef = atof(err);
    } while (errCoef <= 0);
    
    // EXEMPLE D'UTILISATION
    
    // ---- Création des données sources
    double enters[2] = {1,1};
    double out[1] = {0};
    DataSource *data = data_new(enters, out, 2, 2, 0);
    double enters2[2] = {1,0};
    double out2[1] = {1};
    DataSource *data2 = data_new(enters2, out2, 2, 2, 1);
    double enters3[2] = {0,1};
    double out3[1] = {1};
    DataSource *data3 = data_new(enters3, out3, 2, 2, 1);
    double enters4[2] = {0,0};
    double out4[1] = {0};
    DataSource *data4 = data_new(enters4, out4, 2, 2, 0);
    
    DataSource *final = malloc(sizeof(DataSource) * 4);
    final[0] = *data;
    final[1] = *data2;
    final[2] = *data3;
    final[3] = *data4;
    // ----
    
    // ---- Création du réseau avec ses caractéristiques
    Network *net = network_new(2, 1, 1, 3);
    // ----
	
    // ---- Enregistrement du reseau

    //Network_to_File(*net);


    
    // ---- Apprentissage du réseau
    teach(net, final, 4, errCoef, 0.1);
    // ----
    
    // ---- Exécution sur des données pour le tester
    compute(net, *data);
    printResult(*net);
    compute(net, *data2);
    printResult(*net);
    compute(net, *data3);
    printResult(*net);
    compute(net, *data4);
    printResult(*net);
    // ----
    
    char response;
    do {
        printf("Do you want save learning ? (Y/N)");
        response = *(getLine(10));
    } while (response != 'Y' && response != 'N');
    
    if (response == 'Y') {
        Network_to_File(*net);
        printf("Saved !\n");
    } else {
        printf("Ignored !\n");
    }
    
    return 0;
}
