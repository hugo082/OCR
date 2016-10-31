//
//  Core.h
//  EpitaOCR
//
//  Created by Sofiene on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Save_h
#define Save_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Core.h"


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
**/
int Network_to_File(Network net);


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
	String -> Double
**/
//double string_to_double(char *str_);

/*
	String vers tableau de double
**/
double *string_to_tab(char *s, int l);

/*
	Retourne un reseau de neurone paramétré selon les valeurs présente dans le fichier de sauvegarde
**/
Network *File_to_Network();






#endif
