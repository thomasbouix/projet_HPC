#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"
#include "utils.h"

int main(void){

  printf("COMPUTE ALL FD STEPS\n\n");
  compute_fd_all_steps(50);

  printf("COMPUTE ALL SD STEPS\n\n");
  compute_sd_all_steps();

  return 0;
}
