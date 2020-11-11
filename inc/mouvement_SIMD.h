#ifndef MOUVEMENT_SIMD_H
#define MOUVEMENT_SIMD_H

  #include "vnrdef.h"
  #include "vnrutil.h"
  #include "simd_macro.h"
  #include "nrutil.h"
  #include "utils.h"
  #include "stdio.h"
  #include "stdlib.h"

  #define N 4
  #define V_MIN 1
  #define V_MAX 254

  vuint8** routine_FrameDifference_SIMD(vuint8** I_t, vuint8** I_t_moins_1, int vi0, int vi1, int vj0, int vj1, int theta);
  void compute_fd_SIMD(int threshold, int save);
  void SigmaDelta_step0_SIMD(uint8*** M_0, uint8*** V_0, int* nrl, int* nrh, int* ncl, int* nch);

#endif
