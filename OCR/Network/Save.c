/* Enregistrement des valeurs du network */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Save.h"
#include "Core.h"
#include "Execute.h"
#include "Display.h"
#include "DataSource.h"

#include <float.h>

void doubleToString(char* str, long double nbr, unsigned int size, unsigned int nbcarfloat)
{
    long x = 0;
 
    /* nb de chiffres avant la virgule */
    while (nbr > 1)
    {
        nbr /= 10;
        x++;
    }
 
    int ret = (int) (nbr+0.1);
    nbr = (nbr-ret) * 10;
 
    if (x == 0)
    {
        *(str++) = '0';
        size--;
    }
    else
    {
        while (x-- && size--)
        {
            ret = (int)(nbr+0.1);
            *(str++) = ret + '0';
            nbr = (nbr-ret) * 10;
        }
    }
 
    if (nbcarfloat > 0)
    {
        /* On ecrit la virgule */
        *(str++) = '.';
        while (nbr > FLT_MIN && size-- && nbcarfloat--)
        {
            ret = (int)(nbr+0.1);
            *(str++) = ret + '0';
            nbr = (nbr-ret) * 10;
        }
    }
    /* On termine la chaine */
    *str = 0;
}


int digit_number(int n){
	int i = 0;
	while(n != 0){
		n = n /10;
		i += 1;
	}
	return i;
}

double *choose_tab(Network net, int l){
	if (l == 0)
		return net.weight;
	if (l == 1)
		return net.A;
	if (l == 2)
		return net.In;
	else
		return net.D;
}

char *tab_to_string(Network net, int n){

	int l = 0;
	if (n == 0){
		l = net.enters * net.nByLayer + net.out * net.nByLayer + (net.hLayers - 1) * (net.nByLayer * net.nByLayer);
	}
	else{
		l = net.enters + net.out + (net.hLayers * net.nByLayer);
	}
	
	int i = 0;
	double *tab = choose_tab(net, n);
	char *s = malloc(sizeof(char) * l * 15);

	//printf("Enter : %i \n", i < l);
	while (i < l){
		double a = *tab;
		char arr[15];
		sprintf(arr, "%f", a);


		//printf("%s - %f\n", arr, a);

		strcat(s, arr);
		strcat(s, "\n\r");
		i += 1;
		tab += 1;
	}
	return s;
}


void Network_to_File(Network net){
	static const char filename[] = "test.txt";
	FILE *file = fopen (filename, "w+");
	if (file  != NULL)
	{
		/*
		char *enters_s = (char *)net.enters;
		char *hLayers_s = (char *)net.hLayers;
		char *nByLayer_s = (char *)net.nByLayer;
		char *out_s = (char *)net.out;
		
		strcat(enter_s, "\n\r ");
		strcat(hLayers_s, "\n\r ");
		strcat(nByLayer_s, "\n\r ");
		strcat(out_s, "\n\r ");*/

		char stop[4] = "\n\r";
		char *enter_s = malloc( sizeof (*enter_s) * digit_number(net.enters));
		sprintf(enter_s, "%d", net.enters);
		strcat(enter_s, stop);
		char *hLayers_s = malloc( sizeof (*hLayers_s) * digit_number(net.hLayers));
        	sprintf(hLayers_s, "%d", net.hLayers);
		strcat(hLayers_s, stop);
		char *nByLayer_s = malloc( sizeof (*nByLayer_s) * digit_number(net.nByLayer));
		sprintf(nByLayer_s,"%d" , net.nByLayer);
		strcat(nByLayer_s, stop);
		char *out_s = malloc( sizeof (*out_s) * digit_number(net.out));
        	sprintf(out_s, "%d", net.out);
		strcat(out_s, stop);

		fputs(enter_s, file);
		fputs(hLayers_s, file);
		fputs(nByLayer_s, file);
		fputs(out_s, file);
		
		fputs(tab_to_string(net, 0), file);
		fputs("S", file);
		fputs("\n\r", file);
		fputs(tab_to_string(net, 1), file);
		 fputs("S", file);
                fputs("\n\r", file);
		fputs(tab_to_string(net, 2), file);
		 fputs("S", file);
                fputs("\n\r", file);
		fputs(tab_to_string(net, 3), file);

		fclose(file);
	}
	else
	{
		perror ( filename );
	}
	return;
}

void File_to_Network()
{
   static const char filename[] = "test.txt";
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
	//int i = 0;
 
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
	double d;
	if( 1==sscanf(line,"%lf",&d) )
		printf("%f\n",d);
	else
		puts("not a double variable\n");
      }
      fclose ( file );
   }
   else
   {
      perror ( filename ); /* why didn't the file open? */
   }
   return;
}

int main(){
//	EXEMPLE D'UTILISATION
    
    
      // ---- Création des données sources
      double enters[2] = {1,1};
      double out[1] = {0};
      DataSource *data = data_new(enters, out, 2, 2, 0);
      double enters2[2] = {1,0};
      double out2[1] = {1};
      DataSource *data2 = data_new(enters2, out2, 2, 2, 1);
      double enters3[2] = {0,1};
      double out3[1] = {1};
      DataSource *data3 = data_new(enters3, out3, 2, 2, 1);
      double enters4[2] = {0,0};
      double out4[1] = {0};
      DataSource *data4 = data_new(enters4, out4, 2, 2, 0);
    
      DataSource *final = malloc(sizeof(DataSource) * 4);
      final[0] = *data;
      final[1] = *data2;
      final[2] = *data3;
      final[3] = *data4;
      // ----
    
      // ---- Création du réseau avec ses caractéristiques
      Network *net = network_new(2, 1, 1, 3);
      // ----
 	    
      // ---- Apprentissage du réseau
      //teach(net, final, 4, 0.1, 0.1);
      // ----
  /* static const char filename[] = "test_2.txt";
   FILE *file = fopen ( filename, "w+" );
   if ( file != NULL )
   {
    	int l = net->enters + net->out + (net->hLayers * net->nByLayer);
        int i = 0;
        double *tab = net->A;
        char *s = malloc(sizeof(*s) * (l));
        while (i < l){
		double a = *(tab + i);
                char arr[sizeof(a)];
                sprintf(arr, "%f", a);
                strcat(arr, "\n\r");
                strcat(s, arr);
		fputs(s, file);
                i += 1;
        }

      fclose ( file );
   }*/



      // ---- Enregistrement du reseau
      //Network_to_File(*net);
      // ----
	
      File_to_Network();
      return 0;
}





















//int main(){
//
//	 
//    // EXEMPLE D'UTILISATION
//    
//    
//    // ---- Création des données sources
//    double enters[2] = {1,1};
//    double out[1] = {0};
//    DataSource *data = data_new(enters, out, 2, 2, 0);
//    double enters2[2] = {1,0};
//    double out2[1] = {1};
//    DataSource *data2 = data_new(enters2, out2, 2, 2, 1);
//    double enters3[2] = {0,1};
//    double out3[1] = {1};
//    DataSource *data3 = data_new(enters3, out3, 2, 2, 1);
//    double enters4[2] = {0,0};
//    double out4[1] = {0};
//    DataSource *data4 = data_new(enters4, out4, 2, 2, 0);
//    
//    DataSource *final = malloc(sizeof(DataSource) * 4);
//    final[0] = *data;
//    final[1] = *data2;
//    final[2] = *data3;
//    final[3] = *data4;
//    // ----
//    
//    // ---- Création du réseau avec ses caractéristiques
//    Network *net = network_new(2, 1, 1, 3);
//    // ----
//	    
//    // ---- Apprentissage du réseau
//    teach(net, final, 4, 0.1, 0.1);
//    // ----
//	
//    // ---- Enregistrement du reseau
//    Network_to_File(*net);
//    // ----
//
//    
//    // ---- Exécution sur des données pour le tester
//    /*compute(net, *data);
//    printResult(*net);
//    compute(net, *data2);
//    printResult(*net);
//    compute(net, *data3);
//    printResult(*net);
//    compute(net, *data4);
//    printResult(*net);*/
//    // ----
//	
//	//Network *z = File_to_Network();
//	return 0;
//}

