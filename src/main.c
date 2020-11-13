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

  compute_sd_SIMD_all_steps(1);

  /*int nrl, nrh, ncl, nch;
  uint8** m = LoadPGM_ui8matrix("output/sd_SIMD/E_t_007.pgm", &nrl, &nrh, &ncl, &nch);
  int height = nrh - nrl + 1;
  int width = nch - ncl + 1;
  vbits** v = convert_to_binary(m, height, width);
	uint8** m2 = convert_from_binary(v, height, width);
  //display_ui8matrix (m,nrl,nrh,ncl,nch, "%d\t", NULL);
  display_ui8matrix (m2,nrl,nrh,ncl,nch, "%d\t", NULL);
  free_ui8matrix(m,  nrl,  nrh,  ncl,  nch);
  free_ui8matrix(m2,  nrl,  nrh,  ncl, nch);
  free_vbitsmatrix(v, height, width);*/

    /*vbits** test;
    size_t width = 384;
    size_t height = 4;
    uint8 **m = ui8matrix(0, 3, 0, width-1);
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 16; j++){
        m[i][j] = (uint8)0;
      }
      for(int j = 16; j < 32; j++){
        m[i][j] = (uint8)255;
      }
      for(int j = 32; j < 64; j++){
        m[i][j] = (uint8)0;
      }
      for(int j = 64; j < 96; j++){
        m[i][j] = (uint8)255;
      }
      for(int j = 96; j < 128; j++){
        m[i][j] = (uint8)0;
      }
      for(int j = 128; j < 144; j++){
        m[i][j] = (uint8)255;
      }
      for(int j = 144; j < 160; j++){
        m[i][j] = (uint8)0;
      }
      for(int j = 160; j < 383; j++){
        m[i][j] = (uint8)255;
      }
      m[i][383] = (uint8)0;
      m[i][128] = (uint8)0;
    }
    test = convert_to_binary(m, height, width);
    display_hexa_vbits_matrix(test, height, width);
    printf("\n\n");

    vbits a0, b0, c0;

    vbits aa0, cc0;
    vbits aa1, cc1;
    vbits aa2, cc2;

    int i = 1;
    int j = 1;

    a0 = vec_load(&test[i-1][j-1]);
    b0 = vec_load(&test[i-1][j-0]);
    c0 = vec_load(&test[i-1][j+1]);

    printf("A0\n");
    display_hexa_vbits(a0);
    printf("\n\n");
    printf("B0\n");
    display_hexa_vbits(b0);
    printf("\n\n");
    printf("C0\n");
    display_hexa_vbits(c0);
    printf("\n\n");

    aa0 = vec_left1_bin(a0, b0);
    printf("AA0\n");
    display_hexa_vbits(aa0);
    printf("\n\n");*/

  /*__m128i val = _mm_set_epi64x(0, 1);

  int height = 3;
  int width = 2;
  vbits** m = (vbits**)vui8matrix (0, height-1, 0, width-1);
  vec_store(&m[0][0],val);
  display_hexa_vbits_matrix(m, height, width*128);*/

  return 0;
}
