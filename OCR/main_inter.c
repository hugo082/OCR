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
