/* Enregistrement des valeurs du network */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Save.h"
#include "Core.h"

int digit_number(int n){
	int i = 0;
	while(n != 0){
		n = n /10;
		i += 1;
	}
	return i;
}

/*char[] tab_to_string(Network net){
	return "";
}*/

int Network_to_File(Network net){

	FILE* fichier = NULL;

	fichier = fopen("test.txt", "r+");

	if (fichier != NULL)
   	 {	char stop[1] = ";";
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
		char *totalH_s = malloc( sizeof (*totalH_s) * digit_number(net.totalH));
                sprintf(totalH_s, "%d", net.totalH);
		strcat(totalH_s, stop);		
		
		char s_1[10] = "[enters=";
		strcat(s_1, enter_s);
		char s_2[10] = "hLayers=";
		strcat(s_2, hLayers_s);
		char s_3[15] = "nBylayer=";
		strcat(s_3, nByLayer_s);
		char s_4[15] = "out=";
		strcat(s_4, out_s);
		char s_5[15] = "totalH=";
		strcat(s_5, totalH_s);
		
		char result[1000] ="";
		strcat(result, s_1);
		strcat(result, s_2);
		strcat(result, s_3);
		strcat(result, s_4);
		strcat(result, s_5);
		printf("%s", result);
		fputs(result, fichier);
		fclose(fichier);
		return 1;
   	 }
   	 else
   	 {
       		printf("%s", "Impossible d'ouvrir le fichier test.txt");
		return 0;
   	 }
}
