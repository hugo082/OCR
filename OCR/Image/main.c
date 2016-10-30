//
//  main.c
//  IMG
//
//  Created by Hugo on 28/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Execute.h"

#define DEBUG 0

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
    searchLetters(path);
#else
    searchLetters("/Users/hugofouquet/Downloads/IMG_SRC/PERSO/TD.jpg");
#endif
    return 0;
}
