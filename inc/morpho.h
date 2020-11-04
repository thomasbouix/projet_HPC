#ifndef MORPHO_H
#define MORPHO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrutil.h"
#include "utils.h"

uint8 ** routine_dilatation3(uint8** image, int nrl, int nrh, int ncl, int nch);
void compute_all_dilatation3(void);

uint8 ** routine_erosion3(uint8** image, int nrl, int nrh, int ncl, int nch);
void compute_all_erosion3(void);

#endif
