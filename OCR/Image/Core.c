//
//  Core.c
//  IMG
//
//  Created by Hugo on 28/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include "Core.h"

#define ACCEPT_ERR 1
#define REFUSE_Y_ERR 2
#define REFUSE_X_ERR 0

/*
 PUBLIC
 Initialisation de SDL
 **/
void init_sdl(void) {
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

/*
 PUBLIC
 Charger une image.
 **/
SDL_Surface* load_image(char *path) {
    SDL_Surface *img;
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    return img;
}

/*
 PRIVATE
 Renvoi les références d'un pixel.
 **/
Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

/*
 PRIVATE
 Retourne la veleur d'un pixel.
 **/
Uint32 _getPixel(SDL_Surface *surface, unsigned x, unsigned y) {
    Uint8 *p = pixelref(surface, x, y);
    switch(surface->format->BytesPerPixel) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
    }
    return 0;
}
/*
 PUBLIC
 Retourne la veleur d'un pixel.
 **/
Uint32 getPixel(SDL_Surface *surface, struct CPoint point) {
    return _getPixel(surface, point.x, point.y);
}

/*
 PUBLIC
 Set la valeur d'un pixel.
 **/
void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel) {
    Uint8 *p = pixelref(surface, x, y);
    switch(surface->format->BytesPerPixel) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

/*
 PUBLIC
 Set la valeur si elle est plus grande.
 **/
void ifMax(int *value, int max) {
    if (*value < max) {
        *value = max;
    }
}

/*
 PUBLIC
 Set la valeur si elle est plus petite.
 **/
void ifMin(int *value, int min) {
    if (*value > min) {
        *value = min;
    }
}

/*
 PUBLIC
 Encadre une lettre en bleu.
 **/
void encadrer(SDL_Surface *img, struct LetterFrame letter) {
    Uint32 pixel = SDL_MapRGBA(img->format, 0, 0, 255, 255);
    int x = letter.minX;
    int y = letter.maxY;
    
    for (x = letter.minX; x <= letter.maxX; x++) { // Top Line
        putpixel(img, x, y, pixel);
    }
    y = letter.minY;
    for (x = letter.minX; x <= letter.maxX; x++) { // Bottom Line
        putpixel(img, x, y, pixel);
    }
    x = letter.minX;
    for (y = letter.minY; y <= letter.maxY; y++) { // Left Line
        putpixel(img, x, y, pixel);
    }
    x = letter.maxX;
    for (y = letter.minY; y <= letter.maxY; y++) { // Right Line
        putpixel(img, x, y, pixel);
    }
}

/*
 Si la Lettre a déjà été enregistré
 **/
int isViewed(struct LetterFrame *letters, int len, struct CPoint *point) {
    for (int i = 0; i < len; i++) {
        struct LetterFrame let = *(letters + i);
        if (let.minX <= point->x && let.maxX >= point->x && let.minY <= point->y && let.maxY >= point->y) {
            point->x = let.maxX;
            return 1;
        }
    }
    return 0;
}
/*
 Si le pixel à déjà été passé
 **/
int isPassed(struct CPoint *pixels, int *len, int x, int y) {
    for (int i = 0; i < *len; i++) {
        struct CPoint p = *(pixels + i);
        if (x == p.x && y == p.y)
            return 1;
    }
    return 0;
}
/*
 Set que le pixel à été passé
 **/
void newPassed(struct CPoint *pixels, int *i, int len, int x, int y)  {
    if (*i < len) {
        (*(pixels + *i)).x = x;
        (*(pixels + *i)).y = y;
        *i += 1;
    } else
        errx(34, "Pixels Array Full. \n   Pixel<%i,%i> can't be added.\n   Array cIndex:%i Len:%i\n", x, y, *i, len);
}

/*
 PRIVATE
 Recursion interne.
 **/
void _recOnLetter (SDL_Surface *img, struct CPoint *pixels, int *len, int *cIndex, int x, int y, int *minX, int *maxX, int *minY, int *maxY, int errOffset) {
    Uint8 r,g,b,a;
    Uint32 pixel = _getPixel(img, x, y);
    SDL_GetRGBA(pixel, img->format, &r, &g, &b, &a);
    
    int w = img->w;
    int h = img->h;
    
    if (isPassed(pixels, len, x, y)) // Deja passé
        return;
    newPassed(pixels, cIndex, *len, x, y);
    if ((r + g + b) > 50) { // En dehors
        if (errOffset > ACCEPT_ERR) // Vide
            return;
        errOffset++;
    } else // In Letter
        errOffset = 0;
    
    if (x - 1 >= 0 && errOffset <= REFUSE_X_ERR) { // Left
        ifMin(minX, x - 1);
        _recOnLetter(img, pixels, len, cIndex, x - 1, y, minX, maxX, minY, maxY, errOffset);
    }
    if (x - 1 >= 0 && y - 1 >= 0 && errOffset <= REFUSE_X_ERR) { // Left Bottom
        ifMin(minX, x - 1);
        ifMin(minY, y - 1);
        _recOnLetter(img, pixels, len, cIndex, x - 1, y - 1, minX, maxX, minY, maxY, errOffset);
    }
    if (x - 1 >= 0 && y + 1 < h && errOffset <= REFUSE_X_ERR) { // Left Top
        ifMin(minX, x - 1);
        ifMax(maxY, y + 1);
        _recOnLetter(img, pixels, len, cIndex, x - 1, y + 1, minX, maxX, minY, maxY, errOffset);
    }
    if (y - 1 >= 0 && errOffset <= REFUSE_Y_ERR) { // Bottom
        ifMin(minY, y - 1);
        _recOnLetter(img, pixels, len, cIndex, x, y - 1, minX, maxX, minY, maxY, errOffset);
    }
    if (y + 1 < h && errOffset <= REFUSE_Y_ERR) { // Top
        ifMax(maxY, y + 1);
        _recOnLetter(img, pixels, len, cIndex, x, y + 1, minX, maxX, minY, maxY, errOffset);
    }
    if (x + 1 < w && y + 1 < h && errOffset <= REFUSE_X_ERR) { // Right Top
        ifMax(maxX, x + 1);
        _recOnLetter(img, pixels, len, cIndex, x + 1, y + 1, minX, maxX, minY, maxY, errOffset);
    }
    if (x + 1 < w && errOffset <= REFUSE_X_ERR) { // Right
        ifMax(maxX, x + 1);
        _recOnLetter(img, pixels, len, cIndex, x + 1, y, minX, maxX, minY, maxY, errOffset);
    }
    if (x + 1 < w && y - 1 >= 0 && errOffset <= REFUSE_X_ERR) { // Right Bottom
        ifMax(maxX, x + 1);
        ifMin(minY, y - 1);
        _recOnLetter(img, pixels, len, cIndex, x + 1, y - 1, minX, maxX, minY, maxY, errOffset);
    }
}

/*
 PUBLIC
 Recursion sur une lettre
 **/
void recOnLetter (SDL_Surface *img, struct CPoint *pixels, int *len, int *cIndex, struct CPoint point, struct LetterFrame *letter) {
    _recOnLetter(img, pixels, len, cIndex, point.x, point.y, &letter->minX, &letter->maxX, &letter->minY, &letter->maxY, 0);
}
