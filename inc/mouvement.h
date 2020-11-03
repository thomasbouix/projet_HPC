#ifndef MOUVEMENT_H
#define MOUVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrutil.h"

// Error if a is true
#define CHECK_ERROR(a)                        \
if (a) {                                      \
	fprintf(stderr, "*** Error :\n" #a "\n"); \
	exit(1);                                  \
}

// renvoie un pointeur sur l'image PGM "différence" calculée
uint8** routine_FrameDifference(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta);
// Stocke dans un output/fd/diffXXX.pgm l'image différence (i1 - i0) avec i1=num_image
void compute_fd(int num_image, int threshold);
// Appel compute_fd() pour tout car3
void compute_all_fd(void);

// Calcule 1 itération de moyenne récursive et renvoie le résultat sous forme de matrice
uint8** routine_Mean(uint8** M_t_moins_1, uint8** I_t, int nrl, int nrh, int ncl, int nch);
// Calcule toutes les images moyennes récursives et les stockes dans output/sd/mean
void compute_all_sd_mean(void);
// M_t() - I_t() stockés dnas output/sd/diff/diff_t_.pgm
void compute_all_sd_diff(void);

#endif
