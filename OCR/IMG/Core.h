//
//  Core.h
//  IMG
//
//  Created by Hugo on 28/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef Core_h
#define Core_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

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
void ifMax(int *value, int max);

/*
 PUBLIC
 Set la valeur si elle est plus petite.
 **/
void ifMin(int *value, int min);

/*
 PUBLIC
 Encadre une lettre en bleu.
 **/
void encadrer(SDL_Surface *img, struct LetterFrame letter);

/*
 PUBLIC
 Check si la position est sur une lettre déjà encadré.
 **/
int isViewed(struct LetterFrame *letters, int len, struct CPoint *point);

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
void recOnLetter (SDL_Surface *img, struct CPoint *pixels, int *len, int *cIndex, struct CPoint point, struct LetterFrame *letter);

#endif /* Core_h */
