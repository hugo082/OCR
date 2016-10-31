/* Enregistrement des valeurs du network */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Save.h"
#include "Core.h"
#include "Execute.h"
#include "Display.h"
#include "DataSource.h"

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
	char *s = malloc(sizeof(*s) * (l));
	//printf("%s", "Valeur de poids : ");
	
	/*char s_[15] = "weights=";
	strcat(s, s_);*/
	while (i < l){
		double a = *tab;
		char arr[sizeof(a)+1];
		sprintf(arr, "%f", a);
		strcat(s, arr);
		strcat(s, "_");
		i += 1;
		tab += 1;
	}	
	//printf("%s", s);
	
	return s;
}

int Network_to_File(Network net){

	FILE* fichier = NULL;

	fichier = fopen("test.txt", "w+");

	if (fichier != NULL){

		char stop[1] = ";";
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
		/*char *totalH_s = malloc( sizeof (*totalH_s) * digit_number(net.totalH));
        sprintf(totalH_s, "%d", net.totalH);
		strcat(totalH_s, stop);*/
		/*char *weight = tab_to_string(net, 0);
		strcat(weight, stop);
		char *A = tab_to_string(net, 1);
		strcat(A, stop);
		char *In = tab_to_string(net,2);
		strcat(In, stop);
		char *D = tab_to_string(net,3);
		strcat(D, stop);*/
	
		
		/*char s_1[10] = "[enters=";
		strcat(s_1, enter_s);
		char s_2[10] = "hLayers=";
		strcat(s_2, hLayers_s);
		char s_3[15] = "nBylayer=";
		strcat(s_3, nByLayer_s);
		char s_4[15] = "out=";
		strcat(s_4, out_s);*/
		/*char s_5[15] = "totalH=";
		strcat(s_5, totalH_s);*/
		/*char s_6[15] = "weight=";
		strcat(s_6, weight);
		char s_7[15] = "A=";
		strcat(s_7, A);
		char s_8[15] = "In=";
		strcat(s_8, In);
		char s_9[15] = "D=";
		strcat(s_9, D);*/
		
		
		/*char result[1000] ="";
		strcat(result, s_1);
		strcat(result, s_2);
		strcat(result, s_3);
		strcat(result, s_4);
		//strcat(result, s_5);
		
		/*strcat(result, s_7);
		strcat(result, s_8);
		strcat(result, s_9);*/
		//printf("%s", result);
		//fputs(result, fichier);//TERMINEZ DE REMPLIR LE FICHIERS AVEC LES VALEURS, PS IL FAUT MODIFIER LA FONCTION DU DESSUS
		fputs("[enters=", fichier);
		fputs(enter_s, fichier);
		fputs("hLayers=", fichier);
		fputs(hLayers_s, fichier);
		fputs("nBylayer_s=", fichier);
		fputs(nByLayer_s, fichier);
		fputs("out=", fichier);
		fputs(out_s, fichier);
		fputs("weight=", fichier);
		fputs(tab_to_string(net, 0), fichier);
		fputs(";A=", fichier);
		fputs(tab_to_string(net, 1), fichier);
		fputs(";In=", fichier);
		fputs(tab_to_string(net, 2), fichier);
		fputs(";D=", fichier);
		fputs(tab_to_string(net, 3), fichier);
		fputc(']', fichier);
		fclose(fichier);
		return 1;
   	 }
   	 else
   	 {
       	printf("%s", "Impossible d'ouvrir le fichier test.txt");
		return 0;
   	 }
}

int string_to_value(char *str_){
	
	while (*str_ != '='){
		str_ += 1;
	}
	str_ += 1;
	int value = 0;
	while (*str_ != ';'){
		value = value * 10;
		if ((*str_ == 48))
			value += 0;
		if ((*str_ == 49))
			value += 1;
		if ((*str_ == 50))
			value += 2;
		if ((*str_ == 51))
			value += 3;
		if ((*str_ == 52))
			value += 4;
		if ((*str_ == 53))
			value += 5;
		if ((*str_ == 54))
			value += 6;
		if ((*str_ == 55))
			value += 7;
		if ((*str_ == 56))
			value += 8;
		if ((*str_ == 57))
			value += 9;
		str_ += 1;
	}
	return value; 	
}

char *File_to_string(){

	FILE* fichier = NULL;
	fichier = fopen("test.txt", "r");
	char *result = malloc (sizeof(*result) * 1000);
	//char res[1] = " ";
	int i = 0;

    if (fichier != NULL){
		
		char caractereActuel ='a';
		char *a = malloc(sizeof(*a)*1);		

        while(caractereActuel != ']'){

            caractereActuel = fgetc(fichier);
			*a = caractereActuel;
			strcat(result, a);
			i += 1;
		}
		
		//printf("%s", result);
		free(a);
		return result;

    }
    else{
        printf("Impossible d'ouvrir le fichier test.txt");
		return "Impossible d'ouvrir le fichier";
    }
}

char *delete_str(char *s){
	
	while (*s != ';'){
		s += 1;
	}
	s+= 1;
	size_t l = strlen(s);
	char *result = malloc(sizeof(*result)*l);
	strcat(result, s);
	return result;
} 

/*double string_to_double(char *str_){
	
	while (*str_ != '='){
		str_ += 1;
	}
	str_ += 1;
	int value = 0;
	while (*str_ != '_'){
		s
		str_ += 1;
		
	}
	return value; 	
}*/

double *string_to_tab(char *s, int l){
	double *tab = malloc(sizeof( double ) * l);
	double *result = tab;
	while (*s != '='){
		s += 1;
	}
	while (*s != ';'){
		double value = 0;//string_to_double(s);
		/*
		double dec = 1;
		while (*s != '.'){
			s += 1;
		}
		while (*s != '_'){
			dec =  dec / 10;
			if ((*s == 48))
				value += dec * 0;
			if ((*s == 49))
				value += dec * 1;
			if ((*s == 50))
				value += dec * 2;
			if ((*s == 51))
				value += dec * 3;
			if ((*s == 52))
				value += dec * 4;
			if ((*s == 53))
				value += dec * 5;
			if ((*s == 54))
				value += dec * 6;
			if ((*s == 55))
				value += dec * 7;
			if ((*s == 56))
				value += dec * 8;
			if ((*s == 57))
				value += dec * 9;
			s += 1;
		}*/
		s += 1;
		*tab = value;
		printf("%f", value);
		tab += 1;
		
	}
	
	return result;
}

Network *File_to_Network(){

	char *s = File_to_string();
	int enter = string_to_value(s);
	s = delete_str(s);
	int hLayers = string_to_value(s);
	s = delete_str(s);
	int nbLayer = string_to_value(s);
	s = delete_str(s);
	int out = string_to_value(s);
	s = delete_str(s);
	int len_w = enter * nbLayer + out * nbLayer + (hLayers - 1) * (nbLayer * nbLayer);
	int len_other = enter + out + (hLayers * nbLayer);
	//printf("%s", s);
	printf("%d", enter);
	printf("%d", hLayers);
	printf("%d", nbLayer);
	printf("%d", out);
	
	Network *net = network_new(enter, out, hLayers, nbLayer);
	//string_to_tab(s, len_w);
	return net;

	
}

int main(){

	 
    // EXEMPLE D'UTILISATION
    
    
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
	/*char stop[1] = ";";
	/*char *weight = tab_to_string(*net, 0);
	strcat(weight, stop);
	printf("%s", weight);
	char *A = tab_to_string(*net, 1);
	strcat(A, stop);
	char *In = tab_to_string(*net,2);
	strcat(In, stop);
	char *D = tab_to_string(*net,3);
	strcat(D, stop);*/
	//tab_to_string(*net, 0);
    // ---- Enregistrement du reseau

    //Network_to_File(*net);
	// ----
/*    
    // ---- Exécution sur des données pour le tester
    compute(net, *data);
    printResult(*net);
    compute(net, *data2);
    printResult(*net);
    compute(net, *data3);
    printResult(*net);
    compute(net, *data4);
    printResult(*net);
    // ----
*/    
	
	//Network *net = network_new(2, 1, 1, 3);
	//Network_to_File(*net);

	//File_to_Network();
	/*
	int n = string_to_value("enters=1234;");
	printf("%d", n);*/
	Network *z = File_to_Network();
	return 0;
}
