//
//  DataSource.h
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef NET_DataSource_h
#define NET_DataSource_h

#include <stdlib.h>

#if __APPLE_XCODE__
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#endif

typedef struct _DataSource DataSource;
struct _DataSource {
    char *identifier;
    SDL_Surface *suface;
    
    int entersCount, outCount;
    double response;
    
    double *enters, *out;
};

/*
 PUBLIC
 Retourne une structure de data
 **/
DataSource *data_new(double enters[], double out[], int eCount, int oCount, int response);

// A enrichir :

// Fonction qui prend une image (d'une lettre) et la converti en data...

#endif /* NET_DataSource_h */
