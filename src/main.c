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

  compute_dilatation("output/sd/E_t_", 5);
  compute_erosion("output/sd/E_t_", 5);

  return 0;
}
