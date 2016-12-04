//
//  ImgTable.h
//  NeuronalNetwork
//
//  Created by Hugo on 03/12/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef ImgTable_h
#define ImgTable_h

#include <stdio.h>
#include "Core.h"

#if __APPLE_XCODE__
#include "SDL2/SDL.h"
#else
#include <SDL.h>
#endif

struct pair {
    SDL_Rect rect;
    char *name;
    SDL_Surface *img;
    struct pair *next;
};

struct table {
    size_t                size, capacity;
    struct pair         **tab;
};

/*
 Init table with capacity.
 **/
struct table* init_table(size_t capacity);

/*
 Insert an image in table.
 **/
int insert_pair(struct table *table, SDL_Surface *img, SDL_Rect rect);

#endif /* ImgTable_h */
