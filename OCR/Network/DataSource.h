//
//  DataSource.h
//  EpitaOCR
//
//  Created by Hugo on 12/10/2016.
//  Copyright © 2016 hfqt. All rights reserved.
//

#ifndef DataSource_h
#define DataSource_h

#include <stdlib.h>

typedef struct _DataSource DataSource;
struct _DataSource {
    int     entersCount;
    int     outCount;
    double   response;
    
    double    *enters;
    double    *out;
};

/*
 PUBLIC
 Retourne une structure de data
 **/
DataSource *data_new(double enters[], double out[], int eCount, int oCount, int response);

// A enrichir :

// Fonction qui prend une image (d'une lettre) et la converti en data...

#endif /* DataSource_h */
