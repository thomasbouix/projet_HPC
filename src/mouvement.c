#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "mouvement.h"

uint8** routine_FrameDifference(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta){

  uint8** E_t = ui8matrix(nrl, nrh, ncl, nch);
  // uint8** O_t = ui8matrix(nrl, nrh, ncl, nch);

  for(int i = nrl; i <= nrh; i++){
    for(int j = ncl; j <= nch; j++){
      if(abs(I_t[i][j] - I_t_moins_1[i][j]) < theta)
        E_t[i][j] = (uint8)0;
      else
        E_t[i][j] = (uint8)1;
      // O_t[i][j] = (uint8)abs(I_t[i][j] - I_t_moins_1[i][j]);
    }
  }
  return E_t;
}
