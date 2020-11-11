#ifndef MOUVEMENT_H
#define MOUVEMENT_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "nrutil.h"
  #include "utils.h"

  #define N 4
  #define V_MIN 1
  #define V_MAX 254

  uint8** routine_FrameDifference(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta);
  void compute_fd(int num_image, int threshold, int save);
  void compute_fd_all_steps(int threshold, int save);

  // initialise M_0 et V_0
  void SigmaDelta_step0(uint8*** M_0, uint8*** V_0, int* nrl, int* nrh, int* ncl, int* nch);
  // calcul M_t et V_t à partir de leurs précédents, et retourne E_t
  uint8** SigmaDelta_1step(uint8** M_t_moins_1, uint8*** M_t_save, uint8** V_t_moins_1, uint8*** V_t_save, uint8** I_t, int nrl, int nrh, int ncl, int nch);
  uint8** SigmaDelta_1step_opti(uint8** M_t_moins_1, uint8*** M_t_save, uint8** V_t_moins_1, uint8*** V_t_save, uint8** I_t, int nrl, int nrh, int ncl, int nch);
  void compute_sd_all_steps(void);

#endif
