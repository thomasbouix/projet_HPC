#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"


int main(void){
  int nrl, nrh, ncl, nch;
  uint8** m1 = LoadPGM_ui8matrix("../car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** m2 = LoadPGM_ui8matrix("../car3/car_3101.pgm", &nrl, &nrh, &ncl, &nch);
  // printf("nrl : %d, nrh : %d, ncl : %d, nch : %d\n\n", nrl, nrh, ncl, nch);

  uint8** E_t = routine_FrameDifference(m2, m1, nrl, nrh, ncl, nch, 50);
  display_ui8matrix(E_t, nrl, nrh, ncl, nch, "%d\t", NULL);

  free(m1);
  free(m2);
  free(E_t);
}
