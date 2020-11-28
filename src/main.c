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
#include "morpho_SIMD.h"

#include "test_morpho.h"
#include "test_morpho_SIMD.h"

#include "bench_mouvement.h"
#include "bench_mouvement_SIMD.h"
#include "bench_morpho.h"
#include "bench_morpho_SIMD.h"


int main(int argc, char * argv[]) {

  test_morpho();
  // test_morpho_SIMD();

  // bench_mouvement();
  // bench_mouvement_SIMD();

  // bench_morpho();
  // bench_morpho_SIMD();

  long nrl = 0;
  long nrh = 4;
  long ncl = 0;
  long nch = 6;

  uint8 ** m = ui8matrix(nrl, nrh, ncl, nch);
  zero_ui8matrix(&m, nrl, nrh, ncl, nch);
  vbits** img_bin = convert_to_binary(m, 5, 7);

  // display_hexa_vbits(img_bin[0][0]);
  // display_hexa_vbits(img_bin[1][0]);
  // display_hexa_vbits(img_bin[2][0]);
  // display_hexa_vbits(img_bin[3][0]);
  // display_hexa_vbits(img_bin[4][0]);

  for (int i=0; i<=4; i++) {
    for (int j=0; j<=127; j++) {
      set_bit(img_bin, i, j, 1);
      assert(get_bit(img_bin, i, j) == 1);
      set_bit(img_bin, i, j, 0);
    }
  }


  printf("----\n");
  display_hexa_vbits(img_bin[0][0]);
  display_hexa_vbits(img_bin[1][0]);
  display_hexa_vbits(img_bin[2][0]);
  display_hexa_vbits(img_bin[3][0]);
  display_hexa_vbits(img_bin[4][0]);

  return 0;
}
