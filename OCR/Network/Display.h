//
//  Display.h
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef NET_Display_h
#define NET_Display_h

#include <stdio.h>
#include "Core.h"
#include "DataSource.h"

/*
 PUBLIC
 Affiche le résultat théorique (plus grosse probabilité)
 */
void printTheoricalResult(Network net, DataSource d);

/*
 PUBLIC
 Affiche les 'count' plus grosse probabilité
 */
void printBestOutput(Network net, int count);

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

#endif /* NET_Display_h */
