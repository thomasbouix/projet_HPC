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

#include "bench_mouvement.h"
#include "bench_mouvement_SIMD.h"

#include "test_morpho.h"



int main(int argc, char * argv[]) {

  // bench_mouvement();
  // bench_mouvement_SIMD();
  // test_morpho();

  //compute_sd_SIMD_all_steps(1);
  //compute_dilatation("output/sd_SIMD/E_t_", 3, 1);
  compute_erosion_3x3_SIMD("output/sd_SIMD/E_t_", 1);

  /*int height = 240;
  int width = 320;
  int nb_vbits_col = ceil((float)width/128);

  int nrl, nrh, ncl, nch;
  uint8** m = LoadPGM_padding_ui8matrix("output/sd_SIMD/E_t_106.pgm", &nrl, &nrh, &ncl, &nch, 1);
  uint8** res_m = erosion(m, height, width, 3);
  //display_ui8matrix(res_m, 0, height-1, 0, width-1, "%d\t", NULL);

  vbits** v = convert_to_binary(m, height, width);
  vbits** res_v = erosion_3x3(v, height, width);
  uint8** m2 = convert_from_binary(res_v, height, width);*/
  //display_ui8matrix(m2, 0, height-1, 0, width-1, "%d\t", NULL);

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
      m[i][255] = (uint8)0;
    }
    test = convert_to_binary(m, height, width);
    display_hexa_vbits_matrix(test, height, width);
    printf("\n\n");

    vbits** res = erosion_3x3(test, height, width);
    display_hexa_vbits_matrix(res, height, width);*/

  return 0;
}
