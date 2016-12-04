//
//  ImgTable.c
//  NeuronalNetwork
//
//  Created by Hugo on 03/12/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include "ImgTable.h"

#define PRECISION 10

/*
void print(struct table *table) {
    printf("TAB : %zu / %zu\n", table->size, table->capacity);
    for (size_t i = 0; i < table->capacity; i++) {
        printf("   %02zu|", i);
        struct paire *buff = (table->tab)[i];
        while (buff != NULL) {
            printf(" (%i,%i) |", buff->rect.x, buff->rect.y);
            buff = buff->next;
        }
        printf("_\n");
    }
}*/

void new_tab(struct table *table) {
    table->size = 0;
    table->tab = malloc(sizeof(struct paire*) * table->capacity);
    for (size_t i = 0; i < table->capacity; i++) {
        (table->tab)[i] = NULL;
    }
}

void realloc_tab(struct table *table) {
    table->capacity *= 2;
    table->tab = realloc(table->tab, sizeof(sizeof(struct paire*) * table->capacity));
    for (size_t i = table->size; i < table->capacity; i++) {
        (table->tab)[i] = NULL;
    }
}

struct paire* create_pair(SDL_Surface *img, SDL_Rect rect, char *name) {
    struct paire *p = malloc(sizeof(struct paire));
    p->img = img;
    p->rect = rect;
    p->name = name;
    p->next = NULL;
    return p;
}

struct table* init_table(size_t capacity) {
    struct table *table = malloc(sizeof(struct table));
    table->capacity = capacity;
    new_tab(table);
    return table;
}

int isInPrecision(int value, int reference) {
    if (value < reference - PRECISION)
        return -1;
    else if (value > reference + PRECISION)
        return 1;
    return 0;
}

void move_table(struct paire **tab, int begin, size_t end) {
    struct paire *buff = *(tab + begin);
    for (int i = begin + 1; i < end; i++) {
        struct paire *tmp = *(tab + i);
        *(tab + i) = buff;
        buff = tmp;
    }
}

void sub_insert_paire(struct table *table, int index, struct paire *paire) {
    struct paire *buff = NULL;
    struct paire *tab = table->tab[index];
    while (tab != NULL && isInPrecision(tab->rect.x, paire->rect.x) < 1) {
        buff = tab;
        tab = tab->next;
    }
    if (buff == NULL)
        table->tab[index] = paire;
    else
        buff->next = paire;
    paire->next = tab;
}

void insert_paire(struct table *table, SDL_Surface *img, SDL_Rect rect, char *name) {
    struct paire *newPaire = create_pair(img, rect, name);
    struct paire **tab = table->tab;
    int i = 0;
    while (i < table->size) {
        struct paire *currentTab = *(tab + i);
        int pos = isInPrecision(currentTab->rect.y, rect.y);
        if (pos == 0) {
            sub_insert_paire(table, i, newPaire);
            return;
        } else if (pos == 1)
            break;
        i++;
    }
    if (table->size >= table->capacity)
        realloc_tab(table);
    if (i < table->size)
        move_table(tab, i, table->capacity);
    *(tab + i) = newPaire;
    table->size++;
}
