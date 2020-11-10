#ifndef UTILS_H
#define UTILS_H

#include "vnrdef.h"

// Error if a is true
#define CHECK_ERROR(a)                        \
if (a) {                                      \
	fprintf(stderr, "*** Error :\n" #a "\n"); \
	exit(1);                                  \
}

#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)

void convert_coding(uint8** img, int nrl, int nrh, int ncl, int nch, int from, int to);
uint8** LoadPGM_padding_ui8matrix(char *filename, int* nrl, int* nrh, int* ncl, int* nch, int padding);
void free_padding_ui8matrix(uint8** img_with_padding, int nrl, int nrh, int ncl, int nch, int padding);
vuint8** LoadPGM_vui8matrix(char *filename, int* si0, int* si1, int* sj0, int* sj1, int* vi0, int* vi1, int* vj0, int* vj1);
uint8** add_borders(uint8 **m, int height, int width, int border);
#endif
