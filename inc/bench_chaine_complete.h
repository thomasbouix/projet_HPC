#ifndef BENCH_CHAINE_COMPLETE_H
#define BENCH_CHAINE_COMPLETE_H

#include "morpho.h"
#include "morpho_SIMD.h"

#include "nrutil.h"
#include "nrdef.h"
#include "utils.h"
#include "vnrutil.h"
#include "vnrdef.h"

void bench_chaine_complete_csv(int taille_min, int taille_max, int pas);
void bench_chaine_complete_scalaire(uint8** img, int taille);
void bench_chaine_complete_SIMD_naif(uint8** img, int taille);
void bench_chaine_complete_SIMD_opti(uint8** img, int taille);
void bench_chaine_complete_SIMD_fusion(uint8** img, int taille);

#endif
