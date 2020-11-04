#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"
#include "utils.h"
#include "morpho.h"

int main(void){

  /*printf("COMPUTE ALL FD STEPS\n\n");
  compute_fd_all_steps(50);

  printf("COMPUTE ALL SD STEPS\n\n");
  compute_sd_all_steps();*/

  // Test vite fait des morpho
  /*uint8** m = ui8matrix(-1, 5, -1, 5);
  m[-1] = m[0];
  m[5] = m[4];
  for(int i = -1; i < 6; i++){
    m[0][i] = 1;
    m[4][i] = 1;
    //m[1][i] = 1;
    //m[3][i] = 1;
  }

  display_ui8matrix(m, -1, 5, -1, 5, "%d\t", NULL);
  //uint8** ret = erosion(m, 5, 5, 3);
  uint8** ret = dilatation(m, 5, 5, 3);
  printf("\n\n");
  display_ui8matrix(ret, 0, 4, 0, 4, "%d\t", NULL);*/
  return 0;
}
