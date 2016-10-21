//
//  Execute.h
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Execute_h
#define Execute_h

#include <stdlib.h>
#include <stdio.h>
#include "DataSource.h"
#include "Core.h"

/*
 PUBLIC
 Execute le réseau pour une data source
 dataLen : Nombre de donnée contenu dans data[]
 errorCoeff : Coefficient d'erreur maximum
 learningCoeff : Coefficient d'apprentissage (~0.1)
 **/
void teach(Network *net, DataSource data[], int dataLen, double errorCoeff, double learningCoeff);

/*
 PUBLIC
 Execute le réseau pour une data source
 **/
void compute(Network *net, DataSource data);

/*
 PUBLIC
 Apprend au réseau avec une data source
 **/
double teachData(Network *net, DataSource data, double alpha);

#endif /* Execute_h */
