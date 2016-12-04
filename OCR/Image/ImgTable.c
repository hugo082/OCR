//
//  ImgTable.c
//  NeuronalNetwork
//
//  Created by Hugo on 03/12/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include "ImgTable.h"

//struct pair {
//    SDL_Rect rect;
//    char *name;
//    SDL_Surface *img;
//    struct pair *next;
//};
//
//struct table {
//    size_t                size, capacity;
//    struct pair         **tab;
//};

void new_tab(struct table *table) {
    table->size = 0;
    table->tab = malloc(sizeof(struct pair*) * table->capacity);
    for (size_t i = 0; i < table->capacity; i++) {
        (table->tab)[i] = NULL;
    }
}

struct table* init_table(size_t capacity) {
    struct table *table = malloc(sizeof(struct table));
    table->capacity = capacity;
    new_tab(table);
    return table;
}

int insert_pair(struct table *table, SDL_Surface *img, SDL_Rect rect) {
    int y_i = 0, x_i = 0;
    struct pair **tab = table->tab;
    while (y_i < rect.y) {
        int y = (*tab)->rect.y;
        
        if (y_i > y ) { // Between error margin
            <#statements#>
        }
        
        
    }
}
