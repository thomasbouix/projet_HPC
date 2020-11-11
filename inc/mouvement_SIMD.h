#ifndef MOUVEMENT_SIMD_H
#define MOUVEMENT_SIMD_H

#include "vnrdef.h"
#include "vnrutil.h"
#include "simd_macro.h"
#include "nrutil.h"
#include "utils.h"
#include "stdio.h"
#include "stdlib.h"

vuint8** routine_FrameDifference_SIMD(vuint8** I_t, vuint8** I_t_moins_1, int vi0, int vi1, int vj0, int vj1, int theta);
void compute_fd_SIMD(int threshold, int save);
void SigmaDelta_step0_SIMD(vuint8*** M_0, vuint8*** V_0, int* vi0, int* vi1, int* vj0, int* vj1);
vuint8** SigmaDelta_1step_SIMD(vuint8** M_t_moins_1, vuint8*** M_t_save, vuint8** V_t_moins_1, vuint8*** V_t_save, vuint8** I_t, int vi0, int vi1, int vj0, int vj1);
void compute_sd_SIMD_all_steps(int save);

#endif
