#ifndef MOUVEMENT_SIMD_H
#define MOUVEMENT_SIMD_H

#include "vnrdef.h"
#include "vnrutil.h"
#include "simd_macro.h"
#include "nrutil.h"
#include "utils.h"
#include "stdio.h"
#include "stdlib.h"

uint8** routine_FrameDifference_SIMD(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta);
void compute_fd_SIMD(int threshold, int save);

#endif
