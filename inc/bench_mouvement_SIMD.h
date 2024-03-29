#ifndef BENCH_MOUVEMENT_SIMD_H
  #define BENCH_MOUVEMENT_SIMD_H

  #include <stdio.h>
  #include <stdlib.h>

  #include "nrutil.h"
  #include "nrdef.h"
  #include "utils.h"
  #include "vnrutil.h"
  #include "vnrdef.h"

  #include "mouvement.h"
  #include "mouvement_SIMD.h"

  void mesure_routine_FD_SIMD(void);
  void mesure_all_fd_SIMD(void);
  void mesure_SigmaDelta_1step_SIMD(void);
  void bench_mouvement_SIMD(void);

  void bench_mouvement_csv(int taille_min, int taille_max, int pas);
  void bench_FD_scalaire(uint8** img0, uint8** img1, int taille);
  void bench_FD_SIMD(uint8** img0, uint8** img1, int taille);
  void bench_SD_scalaire(uint8** M_0, uint8** I_1, int taille);
  void bench_SD_SIMD(uint8** M_0, uint8** I_1, int taille);

#endif
