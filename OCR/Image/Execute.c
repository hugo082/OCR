//
//  Execute.c
//  IMG
//
//  Created by Hugo on 29/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include "Execute.h"

#define DEBUG 1

/*
 PUBLIC
 Cherche les lettres dans une image.
 **/
void searchLetters(char *path) {

#if DEBUG == 1
    printf("Searching...\n");
#endif
    
    int len = 0;
    int lettersI = 0;
    struct LetterFrame *letters = malloc(sizeof(int) * 4 * len);
    
    init_sdl();
    SDL_Surface *img = load_image(path);
    
    int w = img->w;
    int h = img->h;
    
    struct CPoint point = {.x = 0, .y = 0};
    for (point.y = 0; point.y < img->h; point.y++) {
        for (point.x = 0; point.x < img->w; point.x++) {
            
            isViewed(letters, len, &point);
            
            Uint8 r,g,b,a;
            Uint32 pixel = getPixel(img, point);
            SDL_GetRGBA(pixel, img->format, &r, &g, &b, &a);
            
            if ((r + g + b) < 50) {
                int pLen = 1024;
                int cIndex = 0;
                
                struct CPoint *pixels = malloc(sizeof(int) * 2 * pLen);
                struct LetterFrame cLetter = {.minX = w, .maxX = 0, .minY = h, .maxY = 0};
                
                recOnLetter(img, pixels, &pLen, &cIndex, point, &cLetter);
                point.x = cLetter.maxX;
                
                len++;
                letters = realloc(letters, sizeof(int) * 4 * len);
                *(letters + lettersI) = cLetter;
                encadrer(img, cLetter);
                lettersI++;
                
                free(pixels);
#if DEBUG == 1
                printf(" Letter founded:\n    x:[%i,%i]\n    y:[%i,%i]\n", cLetter.minX, cLetter.maxX, cLetter.minY, cLetter.maxY);
#endif
            }
        }
    }
    SDL_SaveBMP(img, "/Users/hugofouquet/Downloads/founded.bmp");
#if DEBUG == 1
    printf("Saved !\n");
#endif
}
