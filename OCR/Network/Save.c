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

Network *File_to_Network()
{
   static const char filename[] = "test.txt";
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      int i = 0;
      int S = 0;
      int S_i = 0;
	int enters;
	int hLayers;
	int nByLayer;
	int out;
	Network *net;
 
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {/*
	if (i < 4){
		int nbr = sscanf(line, "%d", &nbr);
		printf("%i", nbr);
		i += 1;
	}
	else{*/
	if (i == 4){
		net = network_new(enters, out, hLayers, nByLayer);
	}
	double d;
	if (i < 4){
		int nbr;
		sscanf(line, "%i", &nbr);
		//printf("%i", nbr);
		if (i == 0)
			enters = nbr;
		if (i == 1)
			hLayers = nbr;
		if (i == 2)
			nByLayer = nbr;
		if (i == 3)
			out = nbr;	
	}	
	else if( 1==sscanf(line,"%lf",&d) ){
		double nd;
		sscanf(line, "%lf", &nd);
		if (S == 0){
			*(net->weight + S_i) = nd;
		}
		if (S == 1){
                        *(net->A + S_i) = nd;
                }
		if (S == 2){
                        *(net->In + S_i) = nd;
                }
		if (S == 3){
                        *(net->D + S_i) = nd;
                }
		S_i += 1;

	}
		//printf("%f\n",d);
	else{
		S += 1;
		S_i = 0;
	}
	i += 1;
      }
      fclose ( file );
      return net;
   }
   else
   {
      perror ( filename ); /* why didn't the file open? */
      return NULL;
   }
}

/*void Print_tab(double *tab, int len){
	int i = 0;
	while (i < len){
		printf("%lf\n", *(tab + i));
		i += 1;
	}
	return;
}*/

/*int main(){
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
      //Network *net = network_new(2, 1, 1, 3);
      // ----
 	    
      // ---- Apprentissage du réseau
      //teach(net, final, 4, 0.1, 0.1);
      // ----

      // ---- Enregistrement du reseau
      //Network_to_File(*net);
      // ----
	
      Network *net = File_to_Network();
      printf("%i\n", net->enters);
      printf("%i\n", net->hLayers);
      printf("%i\n", net->nByLayer);
      printf("%i\n", net->out);
      int h = net->enters + net->out + (net->hLayers * net->nByLayer);
      int nbWeight =  net->enters * net->nByLayer + net->out * net->nByLayer + (net->hLayers - 1) * (net->nByLayer * net->nByLayer);
      printf("%s\n", "weight :");
      Print_tab(net->weight, nbWeight);
      printf("%s\n", "A :");
      Print_tab(net->A, h);
      printf("%s\n", "In :");
      Print_tab(net->In, h);
      printf("%s\n", "D :");
      Print_tab(net->D, h);
      return 0;
}*/
