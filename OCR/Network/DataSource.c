//
//  DataSource.c
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//
//  Pour la description des fonctions :
//  voir le fichier DataSource.h
//

#include "DataSource.h"

DataSource *data_new(double enters[], double out[], int eCount, int oCount, int response) {
    DataSource *data = malloc(sizeof(DataSource));
    data->enters = enters;
    data->out = out;
    data->entersCount = eCount;
    data->outCount = oCount;
    data->response = response;
    return data;
}
