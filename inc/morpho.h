#ifndef MORPHO_H
#define MORPHO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrutil.h"
#include "utils.h"

// OR sur le voisinage
uint8 ** routine_dilatation3(uint8** image, int nrl, int nrh, int ncl, int nch);
void compute_all_dilatation3(void);

// AND sur le voisinage
uint8 ** routine_erosion3(uint8** image, int nrl, int nrh, int ncl, int nch);
void compute_all_erosion3(void);

// dilation(erosion(X))
uint8 ** routine_ouverture3(uint8** image, int nrl, int nrh, int ncl, int nch);
void compute_all_ouverture3(void);

// erosion(dilatation(X))
uint8 ** routine_fermture3(uint8** image, int nrl, int nrh, int ncl, int nch);
void compute_all_fermture3(void);

#endif
