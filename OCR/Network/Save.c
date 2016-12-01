/* Enregistrement des valeurs du network */


#include "Save.h"

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

double *choose_tab(Network net, int l){
    if (l == 0)
        return net.weight;
    if (l == 1)
        return net.A;
    else
        return net.In;
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
    char *s = malloc(sizeof(char) * l * 500);
    while (i < l){
        double a = *tab;
        char arr[500];
        sprintf(arr, "%f", a);
        strcat(s, arr);
        strcat(s, "\n");
        i += 1;
        tab += 1;
    }
    return s;
}


int network_to_file(Network net, char *path){
    FILE *file = fopen (path, "w+");
    if (file  != NULL)
    {
        char enter_s[10], out_s[10], hLayers_s[10], nByLayer_s[10];

        sprintf(enter_s, "%i\n", net.enters);
        sprintf(hLayers_s, "%i\n", net.hLayers);
        sprintf(nByLayer_s,"%i\n" , net.nByLayer);
        sprintf(out_s, "%i\n", net.out);
        
        fputs(enter_s, file);
        fputs(hLayers_s, file);
        fputs(nByLayer_s, file);
        fputs(out_s, file);
        
        fputs(tab_to_string(net, 0), file);
        fputs("S\n", file);
        fputs(tab_to_string(net, 1), file);
        fputs("S\n", file);
        fputs(tab_to_string(net, 2), file);
        fclose(file);
    } else {
        perror (path);
        return 0;
    }
    return 1;
}

Network *file_to_network(char *path) {
    FILE *file = fopen ( path, "r" );
    if ( file != NULL )
    {
        char line [ 128 ]; /* or other suitable maximum line size */
        int i = 0;
        int S = 0;
        int S_i = 0;
        int enters = -1;
        int hLayers = -1;
        int nByLayer = -1;
        int out = -1;
        Network *net = NULL;
        
        while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
        {
            double d;
            if (i == 4){
                if (enters == -1 || out == -1 || hLayers == -1 || nByLayer == -1) {
                    warnx("Error on load network with file : %s", path);
                    return NULL;
                }
                net = network_new(enters, out, hLayers, nByLayer);
            }
            if (i < 4){
                int nbr;
                sscanf(line, "%i", &nbr);
                if (i == 0)
                    enters = nbr;
                if (i == 1)
                    hLayers = nbr;
                if (i == 2)
                    nByLayer = nbr;
                if (i == 3)
                    out = nbr;
            } else if(i > 3 && 1 == sscanf(line,"%lf",&d)) {
                if (S == 0)
                    net->weight[S_i] = d;
                else if (S == 1)
                    net->A[S_i]  = d;
                else if (S == 2)
                    net->In[S_i]  = d;
                S_i += 1;
            } else {
                S++; S_i = 0;
            }
            i++;
        }
        fclose (file);
        return net;
    } else {
        perror (path);
        return NULL;
    }
}
