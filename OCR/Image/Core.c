//
//  Core.c
//  IMG
//
//  Created by Hugo on 28/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//
//  Pour la description des fonctions :
//  voir le fichier Core.h
//

#include "Core.h"

#define ACCEPT_ERR 1
#define REFUSE_Y_ERR 2
#define REFUSE_X_ERR 0

void init_sdl(void) {
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path) {
    SDL_Surface *img;
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    return img;
}

Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y) {
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

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

Uint32 getPixel(SDL_Surface *surface, struct CPoint point) {
    return _getPixel(surface, point.x, point.y);
}

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

void ifMax(int *value, int max) {
    if (*value < max) {
        *value = max;
    }
}

void ifMin(int *value, int min) {
    if (*value > min) {
        *value = min;
    }
}

void encadrer(SDL_Surface *img, SDL_Rect rect) {
    Uint32 pixel = SDL_MapRGBA(img->format, 0, 0, 255, 255);
    int x = rect.x;
    int y = rect.y;
    int maxX = rect.x + rect.w;
    int maxY = rect.y + rect.h;
    
    for (x = rect.x; x <= maxX; x++) { // Top Line
        putpixel(img, x, y, pixel);
    }
    y = maxY;
    for (x = rect.x; x <= maxX; x++) { // Bottom Line
        putpixel(img, x, y, pixel);
    }
    x = rect.x;
    for (y = rect.y; y <= maxY; y++) { // Left Line
        putpixel(img, x, y, pixel);
    }
    x = maxX;
    for (y = rect.y; y <= maxY; y++) { // Right Line
        putpixel(img, x, y, pixel);
    }
}

int isViewed(SDL_Rect *rects, int len, struct CPoint *point) {
    for (int i = 0; i < len; i++) {
        SDL_Rect rect = *(rects + i);
        int maxX = rect.x + rect.w, maxY = rect.y + rect.h;
        if (rect.x <= point->x && maxX >= point->x && rect.y <= point->y && maxY >= point->y) {
            point->x = maxX;
            return 1;
        }
    }
    return 0;
}

int isPassed(struct CPoint *pixels, int *len, int x, int y) {
    for (int i = 0; i < *len; i++) {
        struct CPoint p = *(pixels + i);
        if (x == p.x && y == p.y)
            return 1;
    }
    return 0;
}

void newPassed(struct CPoint *pixels, int *i, int len, int x, int y)  {
    if (*i < len) {
        (*(pixels + *i)).x = x;
        (*(pixels + *i)).y = y;
        *i += 1;
    } else
        errx(34, "Pixels Array Full. \n   Pixel<%i,%i> can't be added.\n   Array cIndex:%i Len:%i\n", x, y, *i, len);
}

void maxX(SDL_Rect *rect, int value) {
    if (rect->x + rect->w < value)
        rect->w = value - rect->x;
}

void minX(SDL_Rect *rect, int value) {
    if (rect->x > value)
        rect->x = value;
}

void maxY(SDL_Rect *rect, int value) {
    if (rect->y + rect->h < value)
        rect->h = value - rect->y;
}

void minY(SDL_Rect *rect, int value) {
    if (rect->y > value)
        rect->y = value;
}


/*
 PRIVATE
 Recursion interne.
 **/
void _recOnLetter (SDL_Surface *img, struct CPoint *pixels, int *len, int *cIndex, int x, int y, SDL_Rect *rect, int errOffset) {
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
        minX(rect, x - 1);
        _recOnLetter(img, pixels, len, cIndex, x - 1, y, rect, errOffset);
    }
    if (x - 1 >= 0 && y - 1 >= 0 && errOffset <= REFUSE_X_ERR) { // Left Bottom
        minX(rect, x - 1);
        minY(rect, y - 1);
        _recOnLetter(img, pixels, len, cIndex, x - 1, y - 1, rect, errOffset);
    }
    if (x - 1 >= 0 && y + 1 < h && errOffset <= REFUSE_X_ERR) { // Left Top
        minX(rect, x - 1);
        maxY(rect, y + 1);
        _recOnLetter(img, pixels, len, cIndex, x - 1, y + 1, rect, errOffset);
    }
    if (y - 1 >= 0 && errOffset <= REFUSE_Y_ERR) { // Bottom
        minY(rect, y - 1);
        _recOnLetter(img, pixels, len, cIndex, x, y - 1, rect, errOffset);
    }
    if (y + 1 < h && errOffset <= REFUSE_Y_ERR) { // Top
        maxY(rect, y + 1);
        _recOnLetter(img, pixels, len, cIndex, x, y + 1, rect, errOffset);
    }
    if (x + 1 < w && y + 1 < h && errOffset <= REFUSE_X_ERR) { // Right Top
        maxX(rect, x + 1);
        _recOnLetter(img, pixels, len, cIndex, x + 1, y + 1, rect, errOffset);
    }
    if (x + 1 < w && errOffset <= REFUSE_X_ERR) { // Right
        maxX(rect, x + 1);
        _recOnLetter(img, pixels, len, cIndex, x + 1, y, rect, errOffset);
    }
    if (x + 1 < w && y - 1 >= 0 && errOffset <= REFUSE_X_ERR) { // Right Bottom
        maxX(rect, x + 1);
        minY(rect, y - 1);
        _recOnLetter(img, pixels, len, cIndex, x + 1, y - 1, rect, errOffset);
    }
}

void recOnLetter (SDL_Surface *img, struct CPoint *pixels, int *len, int *cIndex, struct CPoint point, SDL_Rect *rect) {
    _recOnLetter(img, pixels, len, cIndex, point.x, point.y, rect, 0);
}
