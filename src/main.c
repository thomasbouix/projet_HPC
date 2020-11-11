#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "nrutil.h"
#include "nrdef.h"
#include "utils.h"
#include "vnrutil.h"
#include "vnrdef.h"

#include "mouvement.h"
#include "mouvement_SIMD.h"
#include "morpho.h"

#include "bench_mouvement.h"
#include "bench_mouvement_SIMD.h"

#include "test_morpho.h"

int main(int argc, char * argv[]) {

  // bench_mouvement();
  // bench_mouvement_SIMD();
  // test_morpho();

  int nrl, nrh, ncl, nch;
  uint8** M_t_moins_1 = NULL;
  uint8** V_t_moins_1 = NULL;
  SigmaDelta_step0(&M_t_moins_1, &V_t_moins_1, &nrl, &nrh, &ncl, &nch);

  uint8** I_t = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);

  uint8** M_t = NULL;
  uint8** V_t = NULL;
  uint8** E_t = NULL;
  E_t = SigmaDelta_1step(M_t_moins_1, &M_t, V_t_moins_1, &V_t, I_t, nrl, nrh, ncl, nch);
  SavePGM_ui8matrix(E_t, nrl, nrh, ncl, nch, "classique");
  uint8** M_t_opti = NULL;
  uint8** V_t_opti = NULL;
  uint8** E_t_opti = NULL;
  E_t_opti = SigmaDelta_1step_opti(M_t_moins_1, &M_t_opti, V_t_moins_1, &V_t_opti, I_t, nrl, nrh, ncl, nch);
  SavePGM_ui8matrix(E_t_opti, nrl, nrh, ncl, nch, "opti");

  for(int i=nrl; i<nrh; i++) {
    for (int j=ncl; j<nch; j++){
      if (E_t[i][j] != E_t_opti[i][j])
        printf("ERROR\n");
    }
  }

  for(int i=nrl; i<nrh; i++) {
    for (int j=ncl; j<nch; j++){
      if (M_t[i][j] != M_t_opti[i][j])
        printf("ERROR\n");
    }
  }

  for(int i=nrl; i<nrh; i++) {
    for (int j=ncl; j<nch; j++){
      if (V_t[i][j] != V_t_opti[i][j])
        printf("ERROR\n");
    }
  }

  return 0;
}
