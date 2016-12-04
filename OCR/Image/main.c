//
//  main.c
//  IMG
//
//  Created by Hugo on 28/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Core.h"
#include "Execute.h"
#include "ImgTable.h"

#if __APPLE__
#define PATH "/Users/hugofouquet/EPITA/IMG_SRC/img_text.jpg"
#else
#define PATH "/home/nubel_r/afs/bourgh_s/OCR/"
#endif

#if __APPLE__
#define PATH "/Users/hugofouquet/EPITA/IMG_SRC/img_text.jpg"
#elif __linux__
#define PATH "~/afs/bourgh_s/OCR/image.jpg"
#elif _WIN32
#define PATH "/Path/For/Windows/img.jpg"
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

char* getLine(int maxSize) {
    int i = 0, c = 0;
    char *buffer = malloc(sizeof(char) * maxSize);
    while ((c = getchar()) != 10 && i < maxSize)
        *(buffer + i++) = c;
    return buffer;
}

int main() {
#if DEBUG < 1
    printf("Path : ");
    char *path = getLine(100);
#else
    char *path = PATH;
#endif
    
    struct table *table = init_table(10);
    
//    int eSize;
//    do {
//        printf("Taille de sortie : ");
//        char *err = getLine(10);
//        eSize = atof(err);
//    } while (eSize <= 0);
    
    init_sdl();
    SDL_Surface *img = load_image(path);
    transformToBlackOrWhite(img, 380);
    //SDL_Surface *img2 = redim(img, eSize, eSize);
    searchLettersWithSurface(img, table);
    SDL_SaveBMP(img, "img_final.jpg");

#if DEBUG > 1
    table_print(table);
#endif
    
    return 0;
}
