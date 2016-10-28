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

//char[] tab_to_string(Network net);


/*
	Sauvegarde tout les parametres dans un fichier texte
**/
int Network_to_File(Network net);

#endif
