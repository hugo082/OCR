#include <gtk/gtk.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include "Network/Core.h"
#include "Network/Execute.h"
#include "Network/Display.h"
#include "Network/Save.h"

#include "Image/Core.h"
#include "Image/Execute.h"
#include "Image/ImgTable.h"

#define eSIZE 12 * 12
#define eOUT 26
#define hLAYERS 1
#define nBY_LAYER 29
#define MIN_ERR 0.7

#define BW_FOLDER_PATH "/Users/hugofouquet/Epita/IMG_SRC/8x8/A-Z_OCR/BW/"
#define POLICE_NAME "_OCR.jpg"

GtkWidget *lbl_h;
GtkWidget *lbl2_h;
GtkWidget *lbl_main;
char *path = NULL;

// Global Variable
Network *net;
DataSource *trainingSet;
size_t taining_set_count = 0;
DataSource *productionSet;
size_t prod_set_count = 0;
struct table *letterTable = NULL;

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

DataSource* surface_to_datasource(SDL_Surface *s, char *name) {
    char c = name[0];
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

DataSource* image_path_to_datasource(char *path, char *name) {
    SDL_Surface *s = image_path_to_surface(path, name);
    return surface_to_datasource(s, name);
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
            //s = redim(s, 12, 12);
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
    int eSize = 8;
#if DEBUG < 2
    do {
        printf("Taille de l'image (largeur ou hauteur) (8) : ");
        char *err = getLine(10);
        eSize = atof(err);
    } while (eSize <= 0);
#endif
    int eOut = 26;
#if DEBUG < 2
    do {
        printf("Nombre de cas (26): ");
        char *err = getLine(10);
        eOut = atof(err);
    } while (eOut <= 0);
#endif
    int hLayers = 1;
#if DEBUG < 2
    do {
        printf("Couches cachées (1) : ");
        char *err = getLine(10);
        hLayers = atof(err);
    } while (hLayers <= 0);
#endif
    int nByLayer = 29;
#if DEBUG < 2
    do {
        printf("Neurones par couche (29) : ");
        char *err = getLine(10);
        nByLayer = atof(err);
    } while (nByLayer <= 0);
#endif
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
    printf("Succeed !\n");
}

void search_letter(char **tokens, size_t size) {
    if (size != 2 || tokens[1] == NULL || !strcmp(tokens[1],"")) {
        printf("☠️  Missing argument : search path\n");
        return;
    }
    letterTable = init_table(5);
    searchLettersWithPath(tokens[1], letterTable);
}
void bw_image(char **tokens, size_t size) {
    if (size != 2 || tokens[1] == NULL || !strcmp(tokens[1],"")) {
        printf("☠️  Missing argument : bw path\n");
        return;
    }
    int sensitivity;
    do {
        printf("Sensitivity (650): ");
        char *sty = getLine(5);
        sensitivity = atof(sty);
    } while (sensitivity <= 0);
    SDL_Surface *img = load_image(tokens[1]);
    if (img != NULL) {
        transformToBlackOrWhite(img, sensitivity);
        SDL_SaveBMP(img, tokens[1]);
    }
    printf("Finished.\n");
}

void get_text(){
    if (letterTable == NULL && net != NULL) {
        printf("☠️  You must search letter in image before and initialize network. See 'help'.\n");
        return;
    }
    char *result = malloc(sizeof(char) * letterTable->size * 50);
    size_t char_i = 0;
    SDL_Rect last_rect;
    for (size_t i = 0; i < letterTable->capacity; i++) {
        struct paire *buff = (letterTable->tab)[i];
        if (buff != NULL)
            last_rect = buff->rect;
        while (buff != NULL) {
            DataSource *d = surface_to_datasource(buff->img, buff->name);
            //            if (last_rect.x + last_rect.w + SAPCE_PRECISION < buff->rect.x) {
            //                result[char_i] = ' ';
            //                char_i++;
            //            }
            compute(net, *d);
            result[char_i] = get_result(*net);
            last_rect = buff->rect;
            buff = buff->next;
            char_i++;
        }
        result[char_i] = '\n';
        char_i++;
    }
    printf("Result :\n%s", result);
}

void help() {
    printf("Commands :\n");
    printf("  ==> 'load' : load all files in folder.\n");
    printf("      > load -env folder\n");
    printf("      where  env : 'training' | 'prod'\n");
    printf("  ==> 'saver' : save or load network in file at path.\n");
    printf("      > saver -action path\n");
    printf("      where  action : 'save' | 'load'\n");
    printf("  ==> 'prepro' : execute pre-traitement on files in folder\n");
    printf("      > prepro folder\n");
    printf("  ==> 'search' : search letters in image at path 'path' and save results in folder Results/\n");
    printf("      > search path\n");
    printf("  ==> 'bw' : transform to black and white image at path 'path'\n");
    printf("      > bw path\n");
    printf("  ==> 'init' : init network. All parameters will be requested afterwards.\n");
    printf("      > init\n");
    printf("  ==> 'teach' : start teaching of network. Env 'training' should be loaded and network initialized.\n");
    printf("      > teach\n");
    printf("  ==> 'compute' : compute network. Env 'prod' should be loaded and network initialized.\n");
    printf("      > compute\n");
    printf("  ==> 'get_text' : return text in image of search. 'search' must be executed before and network initialized.\n");
    printf("      > get_text\n");
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
    } else if (strcmp(tokens[0],"get_text") == 0) {
        get_text();
    } else if (strcmp(tokens[0],"saver") == 0) {
        saver(tokens, size);
    } else if (strcmp(tokens[0],"search") == 0) {
        search_letter(tokens, size);
    } else if (strcmp(tokens[0],"bw") == 0) {
        bw_image(tokens, size);
    } else if (strcmp(tokens[0],"help") == 0) {
        help();
    } else
        printf("Command '%s' not found. \n", buffer);
    if (tokens != NULL)
        free(tokens);
    free(buffer);
    return 1;
}

void print_net(Network net) {
    int len = net.enters + net.out + (net.hLayers * net.nByLayer);
    //int wLen =  net.enters * net.nByLayer + net.out * net.nByLayer + (net.hLayers - 1) * (net.nByLayer * net.nByLayer);
    printf("E : %i, O : %i, hLayer : %i, nByLayer : %i\n\n", net.enters, net.out, net.hLayers, net.nByLayer);
    printf("A : [ ");
    for (int i = 0; i < len; i++) {
        printf("%f - ", net.A[i]);
    }
    printf(" ]\n\n");
    printf("In : [ ");
    for (int i = 0; i < len; i++) {
        printf("%f - ", net.In[i]);
    }
    printf(" ]\n\n");
    printf("Weight : [ ");
    for (int i = 0; i < 20; i++) {
        printf("%f - ", net.weight[i]);
    }
    printf(" ]\n\n");
}


int main(int argc, char *argv[])

{

    GtkBuilder      *builder; 

    GtkWidget       *window;

 

    gtk_init(&argc, &argv);

 

    builder = gtk_builder_new();

    gtk_builder_add_from_file (builder, "Interface/glade/window_main.glade", NULL);

 

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    gtk_builder_connect_signals(builder, NULL);

 
    lbl_h = GTK_WIDGET(gtk_builder_get_object(builder, "lbl"));
    lbl2_h = GTK_WIDGET(gtk_builder_get_object(builder, "lbl2"));
    lbl_main = GTK_WIDGET(gtk_builder_get_object(builder, "lbl1"));
    g_object_unref(builder);

 

    gtk_widget_show(window);                

    gtk_main();

    

    return 0;

}

void on_filechoose_file_set(GtkFileChooser *filechoose, GtkWidget *img)
{
	path = gtk_file_chooser_get_preview_filename(filechoose);
	gtk_label_set_text(GTK_LABEL(lbl_h),path);
	gtk_image_set_from_file(GTK_IMAGE(img), path);

} 

// called when window is closed

void on_window_main_destroy()
{

    gtk_main_quit();

}


void on_ocr_clicked(GtkButton *random, GtkWidget *img)
{
    
	gtk_label_set_text(GTK_LABEL(lbl2_h), "searhcing...");
    char **tokens = malloc(sizeof(char *) * 2);
    tokens[0] = "search";
    tokens[1] = path;
    search_letter(tokens, 2);
    
    init_network();
    
    load_train_image("../jeu_img/train");
    start_teaching();
    get_text();
    gtk_label_set_text(GTK_LABEL(lbl2_h), "Finished");

}
