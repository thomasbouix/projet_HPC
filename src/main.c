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
#include <time.h>

int main(void){

  printf("COMPUTE ALL FD STEPS\n\n");

  clock_t begin0 = clock();
  compute_fd_all_steps(50, 0);
  clock_t end0 = clock();
  printf("fd scalaire : %ld ms\n", (end0 - begin0)*1000 / CLOCKS_PER_SEC);

  clock_t begin1 = clock();
  compute_fd_SIMD(50, 0);
  clock_t end1 = clock();
  printf("fd SIMD : %ld ms\n", (end1 - begin1)*1000 / CLOCKS_PER_SEC);

  /*printf("COMPUTE ALL SD STEPS\n\n");
  compute_sd_all_steps();

  compute_dilatation("output/sd/E_t_", 5, 0);
  compute_erosion("output/sd/E_t_", 5, 1);*/


  return 0;
}
