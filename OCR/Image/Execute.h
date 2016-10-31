//
//  Execute.h
//  IMG
//
//  Created by Hugo on 29/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Execute_h
#define Execute_h

#include <stdio.h>
#include "Core.h"
#include <SDL.h>

/*
 PUBLIC
 Cherche les lettres dans une image.
 Prends le chemin de l'image.
 **/
void searchLettersWithPath(char *path);

/*
 PUBLIC
 Cherche les lettres dans une image.
 Prends la surface.
 **/
void searchLettersWithSurface(SDL_Surface *img);

/*
 PUBLIC
 Cherche les lettres dans une image.
 Prends la surface.
 **/
//void transformToBlackAndWhite(void);

#endif /* Execute_h */
