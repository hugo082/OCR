//
//  Display.h
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Display_h
#define Display_h

#include <stdio.h>
#include "Core.h"

/*
 PUBLIC
 Affiche le resultat du réseau
 **/
void printResult(Network net);

/*
 PUBLIC
 Affiche les valeurs du réseau
 **/
void printValue(Network net);

/*
 PUBLIC
 Affiche les deltas du réseau
 **/
void printDeltas(Network net);

/*
 PUBLIC
 Affiche les poids du réseau
 **/
void printWeight(Network net);

#endif /* Display_h */
