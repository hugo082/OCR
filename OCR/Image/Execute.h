//
//  Execute.h
//  IMG
//
//  Created by Hugo on 29/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef IMG_Execute_h
#define IMG_Execute_h

#include <stdio.h>
#include "Core.h"

#if __APPLE_XCODE__
#include "SDL2/SDL.h"
#else
#include <SDL.h>
#endif

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

#endif /* IMG_Execute_h */
