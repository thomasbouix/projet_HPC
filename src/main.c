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

  //compute_sd_SIMD_all_steps(1);

  /*int nrl, nrh, ncl, nch;
  uint8** m = LoadPGM_ui8matrix("output/sd_SIMD/E_t_007.pgm", &nrl, &nrh, &ncl, &nch);
  int height = nrh - nrl + 1;
  int width = nch - ncl + 1;
  vbits** v = convert_to_binary(m, height, width);
	uint8** m2 = convert_from_binary(v, height, width);
  display_ui8matrix (m,nrl,nrh,ncl,nch, "%d\t", NULL);
  display_ui8matrix (m2,nrl,nrh,ncl,nch, "%d\t", NULL);
  free_ui8matrix(m,  nrl,  nrh,  ncl,  nch);
  free_ui8matrix(m2,  nrl,  nrh,  ncl, nch);
  free_vbitsmatrix(v, height, width);*/
  return 0;
}
