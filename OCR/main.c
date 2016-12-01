//
//  main.c
//  EpitaOCR
//
//  Created by Hugo on 05/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include "Network/Core.h"
#include "Network/Execute.h"
#include "Network/Display.h"
#include "Network/Save.h"

#include "Image/Core.h"
#include "Image/Execute.h"

#define eSIZE 8 * 8
#define eOUT 26
#define hLAYERS 1
#define nBY_LAYER 29
#define MIN_ERR 0.7

#define BW_FOLDER_PATH "/Users/hugofouquet/Epita/IMG_SRC/8x8/A-Z_OCR/BW/"
#define POLICE_NAME "_OCR.jpg"

// Global Variable
Network *net;
DataSource *trainingSet;
size_t taining_set_count = 0;
DataSource *productionSet;
size_t prod_set_count = 0;

char* getLine(int maxSize) {
    int i = 0, c = 0;
    char *buffer = malloc(sizeof(char) * maxSize + 1);
    while ((c = getchar()) != 10 && i < maxSize)
        *(buffer + i++) = c;
    *(buffer + i++) = '\0';
    return buffer;
}

char* concat(char *path, char letter, char *name) {
    size_t pLen = strlen(path);
    size_t nLen = strlen(name);
    size_t len = pLen + nLen + 1;
    char *fullname = malloc(sizeof(char) * (len + 1));
    for (size_t i = 0; i < pLen; i++) {
        *(fullname + i) = *(path + i);
    }
    *(fullname + pLen) = letter;
    for (size_t i = pLen + 1; i < len; i++) {
        *(fullname + i) = *(name + i - pLen - 1);
    }
    *(fullname + len) = '\0';
    return fullname;
}

char** strsplit(const char* str, const char* delim, size_t* numtokens) {
    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *strtok_ctx;
    for (token = strtok_r(s, delim, &strtok_ctx);
         token != NULL;
         token = strtok_r(NULL, delim, &strtok_ctx)) {
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }
    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
}

// ======= Command Execution

char** load_files(char *path, size_t *size) {
    char **files = malloc(sizeof(char*) * 1);
    *size = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *name = dir->d_name;
            if (name[0] == '.' || strstr(name, ".") == NULL)
                continue;
            *size += 1;
            files = realloc(files, *size * sizeof(char*));
            files[*size - 1] = strdup(name);
        }
        closedir(d);
    }
    return files;
}

SDL_Surface* image_path_to_surface(char *path, char *name) {
    size_t path_len = strlen(path);
    if (path[path_len - 1] != '/') {
        path_len += 1;
        path = realloc(path, path_len);
        path[path_len - 1] = '/';
        path[path_len] = '\0';
    }
    
    size_t full_name_len = path_len + strlen(name) + 1;
    char *fullName = malloc(sizeof(char) * full_name_len);
    strcpy(fullName, path);
    strcat(fullName, name);
    fullName[full_name_len] = '\0';
    SDL_Surface *s = load_image(fullName);
    free(fullName);
    return s;
}

DataSource* image_path_to_datasource(char *path, char *name) {
    char c = name[0];
    SDL_Surface *s = image_path_to_surface(path, name);
    if (s == NULL)
        return NULL;
    double *enters = load_enters(s);
    double *out = malloc(sizeof(double) * eOUT);
    for (int l = 'A'; l <= 'Z'; l++) {
        int delta = l - 'A';
        out[delta] = l == c;
    }
    DataSource *d = data_new(enters, out, s->h * s->w, eOUT, 0);
    d->identifier = name;
    d->suface = s;
    return d;
}

void load_train_image(char *path) {
    size_t size;
    char** files = load_files(path, &size);
    trainingSet = malloc(sizeof(DataSource) * size);
    taining_set_count = 0;
    for (size_t i = 0; i < size; i++) {
        DataSource *d = image_path_to_datasource(path, files[i]);
        if (d != NULL)
            trainingSet[taining_set_count++] = *d;
    }
    printf("%zu files loaded in train environnement.\n", taining_set_count);
    free(files);
}

void load_prod_image(char *path) {
    size_t size;
    char** files = load_files(path, &size);
    productionSet = malloc(sizeof(DataSource) * size);
    prod_set_count = 0;
    for (size_t i = 0; i < size; i++) {
        DataSource *d = image_path_to_datasource(path, files[i]);
        if (d != NULL)
            productionSet[prod_set_count++] = *d;
    }
    printf("%zu files loaded in production environnement.\n", prod_set_count);
    free(files);
}

void loader(char **tokens, size_t size) {
    if (size != 3 || tokens[1] == NULL || !strcmp(tokens[1],"") || tokens[2] == NULL || !strcmp(tokens[2],"")) {
        printf("☠️  Missing argument : load -env folder\n    env : 'training' | 'prod'\n");
        return;
    }
    if (strcmp(tokens[1],"-training") == 0)
        load_train_image(tokens[2]);
    else if (strcmp(tokens[1],"-prod") == 0)
        load_prod_image(tokens[2]);
    else
        printf("Env '%s' invalid. Use -training or -prod \n", tokens[1]);
}


void preprocessing_image(char **tokens, size_t size) {
    if (size != 2 || tokens[1] == NULL || !strcmp(tokens[1],"")) {
        printf("☠️  Missing argument : prepro folder\n");
        return;
    }
    int sensitivity;
    do {
        printf("Sensitivity : ");
        char *sty = getLine(5);
        sensitivity = atof(sty);
    } while (sensitivity <= 0);
    char *path = tokens[1]; size_t path_len = strlen(path);
    size_t files_count;
    char** files = load_files(path, &files_count);
    size_t count = 0;
    for (size_t i = 0; i < files_count; i++) {
        SDL_Surface *s = image_path_to_surface(path, files[i]);
        if (s != NULL) {
            count++;
            transformToBlackOrWhite(s, sensitivity);
            char *dest = malloc(sizeof(char) * path_len + 50);
            strcpy(dest, path);
            strcat(dest, "BW/");
            mkdir(dest, 0777);
            strcat(dest, files[i]);
            SDL_SaveBMP(s, dest);
        }
    }
    printf("%zu files preprecessed in BW/ folder.\n", count);
    free(files);
}

void init_network(){
    int eSize;
    do {
        printf("Taille de l'image (largeur ou hauteur) (8) : ");
        char *err = getLine(10);
        eSize = atof(err);
    } while (eSize <= 0);
    int eOut;
    do {
        printf("Nombre de cas (26): ");
        char *err = getLine(10);
        eOut = atof(err);
    } while (eOut <= 0);
    int hLayers;
    do {
        printf("Couches cachées (1) : ");
        char *err = getLine(10);
        hLayers = atof(err);
    } while (hLayers <= 0);
    int nByLayer;
    do {
        printf("Neurones par couche (29) : ");
        char *err = getLine(10);
        nByLayer = atof(err);
    } while (nByLayer <= 0);
    net = network_new(eSize * eSize, eOut, hLayers, nByLayer);
    printf("Réseau créé !\n");
}

void start_teaching(){
    if (net == NULL || taining_set_count < 1 || trainingSet == NULL) {
        printf("☠️  Error.\n     Network not initialized\n  OR training set not loaded.\n");
        return;
    }
    
    double errCoef;
    do {
        printf("Max Error : ");
        char *err = getLine(10);
        errCoef = atof(err);
    } while (errCoef <= 0);
    teach(net, trainingSet, taining_set_count, errCoef, 0.1);
}

void compute_prod(){
    if (net == NULL || prod_set_count < 1 || productionSet == NULL) {
        printf("☠️  Error.\n     Network not initialized\n  OR production set not loaded.\n");
        return;
    }
    for (size_t i = 0; i < prod_set_count; i++) {
        DataSource d = productionSet[i];
        compute(net, d);
        printTheoricalResult(*net, d);
    }
    printf("%zu data computed.\n", prod_set_count);
}

void saver(char **tokens, size_t size) {
    if (size != 3 || tokens[1] == NULL || !strcmp(tokens[1],"") || tokens[2] == NULL || !strcmp(tokens[2],"")) {
        printf("☠️  Missing argument : saver -action path\n    action : 'save' | 'load'\n");
        return;
    }
    if (strcmp(tokens[1],"-save") == 0) {
        if (net == NULL) {
            printf("☠️  Error.\n     Network not initialized.\n");
            return;
        }
        network_to_file(*net, tokens[2]);
    } else if (strcmp(tokens[1],"-load") == 0)
        net = file_to_network(tokens[2]);
    else
        printf("Env '%s' invalid. Use -save or -load \n", tokens[1]);
    printf("Succed !\n");
}

void help() {
    printf("Commands :\n");
    printf("  ==> 'load' : load all files in subfolder 'BW' of folder.\n");
    printf("      > load -env folder\n");
    printf("      where  env : 'training' | 'prod'\n");
    printf("  ==> 'saver' : save or load network in file at path.\n");
    printf("      > saver -action path\n");
    printf("      where  action : 'save' | 'load'\n");
    printf("  ==> 'prepro' : execute pre-traitement on files in folder\n");
    printf("      > prepro folder\n");
    printf("  ==> 'init' : init network. All parameters will be requested afterwards.\n");
    printf("      > init\n");
    printf("  ==> 'teach' : start teaching of network. Env 'training' should be loaded and network initialized.\n");
    printf("      > teach\n");
    printf("  ==> 'compute' : compute network. Env 'prod' should be loaded and network initialized.\n");
    printf("      > compute\n");
    printf("  ==> 'exit' : exit shell.\n");
    printf("      > exit\n");
    printf("Good luck ! 😘\n");
}

// =======



int check_command(char *buffer) {
    if (strcmp(buffer, "") == 0)
        return 1;
    size_t size;
    char** tokens = strsplit(buffer, " ", &size);
    if (strcmp(tokens[0],"exit") == 0) {
        return 0;
    } else if (strcmp(tokens[0],"load") == 0) {
        loader(tokens, size);
    } else if (strcmp(tokens[0],"prepro") == 0) {
        preprocessing_image(tokens, size);
    } else if (strcmp(tokens[0],"teach") == 0) {
        start_teaching();
    } else if (strcmp(tokens[0],"init") == 0) {
        init_network();
    } else if (strcmp(tokens[0],"compute") == 0) {
        compute_prod();
    } else if (strcmp(tokens[0],"saver") == 0) {
        saver(tokens, size);
    } else if (strcmp(tokens[0],"help") == 0) {
        help();
    } else
        printf("Command '%s' not found. \n", buffer);
    if (tokens != NULL)
        free(tokens);
    free(buffer);
    return 1;
}

int debug_main(){
    init_sdl();
    int size = 'Z' - 'A';
    DataSource *trainingSet = malloc(sizeof(DataSource) * size);
    for (char c = 'A'; c <= 'Z'; c++) {
        int index = c - 'A';
        char *fullName = concat(BW_FOLDER_PATH, c, POLICE_NAME);
        SDL_Surface *s = load_image(fullName);
        //        transformToBlackOrWhite(s, 400);
        //        char *newName = concat( "/Users/hugofouquet/Epita/IMG_SRC/8x8/OCR/", c, "_OCR_BW.jpg");
        //        SDL_SaveBMP(s, newName);
        int sSize = s->h * s->w;
        double *enters = malloc(sizeof(double) * sSize);
        for (int y = 0; y < s->h; y++) {
            int delta = s->w * y;
            for (int x = 0; x < s->w; x++) {
                struct CPoint p = {.x=x, .y=y};
                Uint8 r,g,b,a;
                Uint32 pixel =getPixel(s, p);
                SDL_GetRGBA(pixel, s->format, &r, &g, &b, &a);
                enters[delta + x] = ((r + g + b) / (3 * 255)) != 0;
            }
        }
        double *out = malloc(sizeof(double) * size);
        for (int l = 'A'; l <= 'Z'; l++) {
            int delta = l - 'A';
            out[delta] = l == c;
        }
        int response = (index) / size;
        DataSource *d = data_new(enters, out, sSize, size, response);
        d->identifier = concat("", c, POLICE_NAME);
        trainingSet[index] = *d;
        free(fullName);
        free(s);
    }
    
    // ---- Création du réseau avec ses caractéristiques + app
    Network *n = network_new(eSIZE, eOUT, hLAYERS, nBY_LAYER);
    teach(n, trainingSet, size, MIN_ERR, 0.1);
    // ----
    
    // ---- Exécution sur des données pour le tester
    for (int _i = 0; _i < size; _i++) {
        DataSource d = trainingSet[_i];
        compute(n, d);
        printTheoricalResult(*n, d);
    }
    // ----
    return 0;
}


int main() {
#if DEBUG > 1
    printf("Debug Version\n");
    return debug_main();
#endif
    printf("Hello !\n   You can use 'help', 'load', 'saver', 'prepro', 'teach', 'compute', 'exit' or 'init' command.\n");
    init_sdl();
    char *cmd = NULL;
    do {
        printf("Enter 👉 ");
        cmd = getLine(150);
    } while (check_command(cmd));
    printf("Good bye ! 🙋‍♂️\n");
    return 0;
}





