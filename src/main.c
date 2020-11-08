#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"
#include "mouvement_SIMD.h"
#include "utils.h"
#include "morpho.h"
#include "vnrutil.h"
#include "vnrdef.h"

int main(void){

  printf("COMPUTE ALL FD STEPS\n\n");
  compute_fd_all_steps(50);

  compute_fd_SIMD(50, 1);

  /*printf("COMPUTE ALL SD STEPS\n\n");
  compute_sd_all_steps();

  compute_dilatation("output/sd/E_t_", 5, 0);
  compute_erosion("output/sd/E_t_", 5, 1);*/


  /*int nrl, nrh, ncl, nch;
  int theta = 50;

  uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** img1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);

  //uint8** res = routine_FrameDifference_SIMD(img1, img0, nrl, nrh, ncl, nch, theta);
  uint8** res_bis = routine_FrameDifference(img1, img0, nrl, nrh, ncl, nch, theta);

  display_ui8matrix(res_bis, nrl, nrh, ncl, nch, "%d\t", NULL);*/


  return 0;
}
