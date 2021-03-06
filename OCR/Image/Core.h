//
//  Core.h
//  IMG
//
//  Created by Hugo on 28/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef IMG_Core_h
#define IMG_Core_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#if __APPLE_XCODE__
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

/*
 PUBLIC
 Cadre de la lettre
 **/
struct LetterFrame {
    int minX, maxX, minY, maxY;
};

/*
 PUBLIC
 Coordonnées d'un point.
 **/
struct CPoint {
    int x, y;
};

/*
 Initialisation de SDL
 **/
void init_sdl(void);

/*
 Charger une image.
 **/
SDL_Surface* load_image(char *path);

/*
 PUBLIC
 Retourne la veleur d'un pixel.
 **/
Uint32 getPixel(SDL_Surface *surface, struct CPoint point);

/*
 PUBLIC
 Set la valeur d'un pixel.
 **/
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

/*
 PUBLIC
 Set la valeur si elle est plus grande.
 **/
void maxX(SDL_Rect *rect, int value);

/*
 PUBLIC
 Set la valeur si elle est plus petite.
 **/
void minX(SDL_Rect *rect, int value);

/*
 PUBLIC
 Set la valeur si elle est plus grande.
 **/
void maxY(SDL_Rect *rect, int value);

/*
 PUBLIC
 Set la valeur si elle est plus petite.
 **/
void minY(SDL_Rect *rect, int value);

/*
 PUBLIC
 Encadre une lettre en bleu.
 **/
void encadrer(SDL_Surface *img, SDL_Rect rect);

/*
 PUBLIC
 Check si la position est sur une lettre déjà encadré.
 **/
int isViewed(SDL_Rect *rects, int len, struct CPoint *point);

/*
 PUBLIC
 Si le pixel à déjà été traité.
 **/
int isPassed(struct CPoint *pixels, int *len, int x, int y);

/*
 PUBLIC
 Indique que le pixel à été traité.
 **/
void newPassed(struct CPoint *pixels, int *i, int len, int x, int y);

/*
 PUBLIC
 Recursion sur une lettre
 **/
void recOnLetter (SDL_Surface *img, struct CPoint *pixels, int *len, int *cIndex, struct CPoint point, SDL_Rect *rect);

/*
 PUBLIC
 Transforme une image en Noir/Blanc.
 Sensibility est le seuil.
 **/
void transformToBlackOrWhite(SDL_Surface *img, int sensitivity);

/*
 PUBLIC
 Redimensionne une image.
 hauteur est la heuteur souhaite, largeur et la largeur souhaite.
 **/
SDL_Surface* redim(SDL_Surface *img, int hauteur, int largeur); 

/*
 PUBLIC
 Retourne les entrées pour le réseaux de neurones.
 */
double* load_enters(SDL_Surface *s);

#endif /* IMG_Core_h */
