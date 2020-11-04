#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"
#include "utils.h"
#include "morpho.h"

int main(void){

  printf("COMPUTE ALL FD STEPS\n\n");
  compute_fd_all_steps(50);

  printf("COMPUTE ALL SD STEPS\n\n");
  compute_sd_all_steps();

  printf("COMPUTE ALL DILATATION3\n\n");
  compute_all_dilatation3();

  printf("COMPUTE ALL EROSION3\n\n");
  compute_all_erosion3();

  return 0;
}
