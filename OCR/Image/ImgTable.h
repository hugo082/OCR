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

struct paire {
    SDL_Rect rect;
    char *name;
    SDL_Surface *img;
    struct paire *next;
};

struct table {
    size_t                size, capacity;
    struct paire         **tab;
};

/*
 Init table with capacity.
 **/
struct table* init_table(size_t capacity);

/*
 Return a new paire.
 **/
struct paire* create_pair(SDL_Surface *img, SDL_Rect rect, char *name);

/*
 Insert an image in table 2D.
 **/
void insert_paire(struct table *table, SDL_Surface *img, SDL_Rect rect, char *name);

/*
 Insert an image in table 1D at index.
 **/
void sub_insert_paire(struct table *table, int index, struct paire *paire);

/*
 Init tab of table.
 **/
void new_tab(struct table *table);

/*
 Realloc memory of tab of table. (x2)
 **/
void realloc_tab(struct table *table);

/*
 Move all elements at right.
 **/
void move_table(struct paire **tab, int begin, size_t end);

/*
 -1 si value est inférieur à l'intervalle de precision de reference
 0 si value est dans à l'intervalle de precision de reference
 1 si value est supérieur à l'intervalle de precision de reference
 **/
int isInPrecision(int value, int reference);

#endif /* ImgTable_h */
