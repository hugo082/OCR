//
//  Core.h
//  EpitaOCR
//
//  Created by Sofiene on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Save_h
#define Save_h

#include "Core.h"
#include "DataSource.h"

#include <float.h>
#include <stdlib.h>

/*
 Retourne le nombre de digits de n
 **/
int digit_number(int n);

/*
 Choisie le tableau que l'on veut sauvegarder
 **/
double *choose_tab(Network net, int l);

/*
 Transforme un tableau de double en tableau de string
 **/
char *tab_to_string(Network net, int n);


/*
 Sauvegarde tout les parametres dans un fichier texte
 Retourne 0 si erreur, 1 sinon
 **/
int network_to_file(Network net, char *path);

/*
 Retourne la valeur int d'une variable enregistré dans le fichier de 	sauvegarde. Prend une chaine de charactère en paramètre.
 **/
int string_to_value(char *str_);

/*
 Retourne la chaine de caractère contenant toute les valeurs du network.
 **/
char *File_to_string();

/*
 Retourne la chaine de caractère contenant les valeurs du network moins la derniere valeur recupéré.
 **/
char *delete_str(char *s);

/*
 Retourne un reseau de neurone paramétré selon les valeurs présente dans le fichier de sauvegarde
 NULL si une erreur est survenue.
 **/
Network *file_to_network(char *path);

#endif
