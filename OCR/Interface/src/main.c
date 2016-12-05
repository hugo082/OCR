#include <gtk/gtk.h>

GtkWidget *lbl_h;
GtkWidget *lbl2_h;

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
    g_object_unref(builder);

 

    gtk_widget_show(window);                

    gtk_main();

    

    return 0;

}

void on_filechoose_file_set(GtkFileChooser *filechoose, GtkWidget *img)
{
	char *path = gtk_file_chooser_get_preview_filename(filechoose);
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

	gtk_label_set_text(GTK_LABEL(lbl2_h), "resul.txt");
	//system("cd ../NNocr/ && ./OcrNN" );
	//system("mv ../NNocr/texte.txt ./");


}
