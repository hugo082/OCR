//
//  Execute.c
//  IMG
//
//  Created by Hugo on 29/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//
//  Pour la description des fonctions :
//  voir le fichier Execute.h
//

#include "Execute.h"

#define DEBUG 1
#define CURRENT_PATH "/Users/hugofouquet/Downloads/"

void extractSurface(SDL_Surface *src, SDL_Rect rect, char *name) {
    SDL_Surface *dst = malloc(sizeof(SDL_Surface));
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dst = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
    if(dst == NULL)
        errx(54, "CreateRGBSurface failed: %s\n", SDL_GetError());
    
    SDL_BlitSurface(src, &rect, dst, &dstrect);
    if (dst == NULL)
        errx(55, "SDL_BlitSurface failed: %s\n", SDL_GetError());
    char *path = malloc(sizeof(char) * 200);
    strcpy(path, CURRENT_PATH);
    strcat(path, "Results/");
    strcat(path, name);
    SDL_SaveBMP(dst, path);
    free(path);
}

void searchLettersWithPath(char *path) {
    init_sdl();
    SDL_Surface *img = load_image(path);
    searchLettersWithSurface(img);
}

void searchLettersWithSurface(SDL_Surface *img) {
    init_sdl();
#if DEBUG > 0
    printf("Searching...\n");
#endif
    
    int len = 0;
    int lettersI = 0;
    SDL_Rect *rects = malloc(sizeof(SDL_Rect) * len);
    
    int w = img->w;
    int h = img->h;
    
    struct CPoint point = {.x = 0, .y = 0};
    for (point.y = 0; point.y < img->h; point.y++) {
        for (point.x = 0; point.x < img->w; point.x++) {
            
            isViewed(rects, len, &point);
            
            Uint8 r,g,b,a;
            Uint32 pixel = getPixel(img, point);
            SDL_GetRGBA(pixel, img->format, &r, &g, &b, &a);
            
            if ((r + g + b) < 50) {
                int pLen = 1024;
                int cIndex = 0;
                
                struct CPoint *pixels = malloc(sizeof(int) * 2 * pLen);
                SDL_Rect cRect = {.x=w, .y=h, .w=0, .h= 0};
                
                recOnLetter(img, pixels, &pLen, &cIndex, point, &cRect);
                point.x = cRect.x + cRect.w;
                
                len++;
                rects = realloc(rects, sizeof(int) * 4 * len);
                *(rects + lettersI) = cRect;
                char *name = malloc(sizeof(char) * 15);
                sprintf(name, "Letter_%i.bmp", lettersI);
                extractSurface(img, cRect, name);
                lettersI++;
                
#if DEBUG > 1
                encadrer(img, cRect);
                printf("Letter %i founded\n", lettersI);
#endif
#if DEBUG > 2
                char *foundedPath = malloc(sizeof(char) * 200);
                strcpy(foundedPath, CURRENT_PATH);
                strcat(foundedPath, "founded_tmp.bmp");
                SDL_SaveBMP(img, foundedPath);
                free(foundedPath);
                getchar();
#endif
                free(pixels);
                free(name);
            }
        }
    }
    char *foundedPath = malloc(sizeof(char) * 200);
    strcpy(foundedPath, CURRENT_PATH);
    strcat(foundedPath, "founded.bmp");
    SDL_SaveBMP(img, foundedPath);
    free(foundedPath);
#if DEBUG > 0
    printf("Finish. %i letters founded.\n", lettersI);
#endif
}

//void transformToBlackAndWhite(SDL_Surface *img) {
//    transform(img, int sensitivity) {
//}
