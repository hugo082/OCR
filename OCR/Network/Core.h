//
//  Core.h
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Core_h
#define Core_h

#include <err.h>
#include <math.h>
#include <stdlib.h>

typedef struct _Network Network;
struct _Network {
    int enters;     // Nombre d'entrée dans le réseau
    int hLayers;    // Nombre de couches cachées
    int nByLayer;   // Nombre de neurone par couche cachées
    int out;        // Nombre de sortie du réseau
    int totalH;     // Nombre de neurone total
    
    double *weight;  // Poids de liaisons ATTENTION: Ne pas le modifier directement
    double *A;       // Valeur des neurones
    double *In;      // In des neurones
    double *D;       // Delta des neurones
};

/*
 PUBLIC
 Retourne un réseau de neurone
 **/
Network *network_new(int enters, int out, int hLayers, int nByLayer);

/*
 PRIVATE
 Initialise le tableau des poids.
 **/
void init_weight(Network *net);

/*
 PRIVATE
 Retoune l'index dans le tableau de poids pour une liaison.
 **/
int weightIndex(Network net, int n1, int n2);

/*
 PUBLIC
 Retoune le poids pour une connexion.
 **/
double getWeight(Network net, int n1, int toN2);

/*
 PUBLIC
 Modifie le poids pour une connexion.
 **/
void setWeight(Network *net, int n1, int toN2, double value);

/*
 PUBLIC
 Retourne la couche du neurone.
 **/
int getLayer(Network net, int neuron);

/*
 PUBLIC
 Retourne la position du neurone sur la couche.
 **/
int nbOfNeuroneInLayer(Network net, int n);

/*
 PUBLIC
 Retourne l'index du Nième neurone de la couche layer.
 Pour neuron de 0...(n-1)
 **/
int iemeNeuronInLayer(Network net, int neuron, int layer);

/*
 PUBLIC
 Indice des neurones de la couche :
 *begin : l'indice du premier
 *end : l'indice du dernier
 /!\ Modification par pointeur
 **/
void getNeurons(Network net, int layer, int *begin, int *end);

/*
 PUBLIC
 Update le in du neurone.
 **/
void updateIn(Network *net, int neuron);

/*
 PUBLIC
 Update le delta du neuron.
 **/
void updateDelta(Network *net, int neuron, double y, double h);

/*
 PUBLIC
 Fonction sigmoïde
 **/
double sigmoide(double x);

/*
 PUBLIC
 Fonction dérivé de sigmoïde
 **/
double sigmoidePrime(double x);

#endif /* Core_h */
