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

  // test_morpho();
  //test_morpho_SIMD();

  //bench_mouvement();
  //bench_mouvement_SIMD();

  //bench_morpho();
  //bench_morpho_SIMD();

  //compute_sd_SIMD_all_steps(1);
  /*clock_t begin0 = clock();
  compute_erosion("output/sd_SIMD/E_t_", 3, 0);
  clock_t end0 = clock();
  printf("erosion scalaire : %ld ms\n", (end0 - begin0)*1000 / CLOCKS_PER_SEC);
  clock_t begin1 = clock();
  compute_all_erosion_3x3_SIMD("output/sd_SIMD/E_t_", 0);
  clock_t end1 = clock();
  printf("erosion SIMD : %ld ms\n", (end1 - begin1)*1000 / CLOCKS_PER_SEC);*/

  /*int height = 240;
  int width = 320;
  int nb_vbits_col = ceil((float)width/128);
  int nrl, nrh, ncl, nch;
  uint8** m = LoadPGM_padding_ui8matrix("output/sd_SIMD/E_t_007.pgm", &nrl, &nrh, &ncl, &nch, 1);
  uint8** res_m = dilatation(m, height, width, 3);
  display_ui8matrix(res_m, 0, height-1, 0, width-1, "%d\t", NULL);
  vbits** v = convert_to_binary(m, height, width);
  vbits** res_v = dilatation_3x3_SIMD(v, height, width);
  uint8** m2 = convert_from_binary(res_v, height, width);
  //display_ui8matrix(m2, 0, height-1, 0, width-1, "%d\t", NULL);*/


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
    vbits** res = erosion_3x3_SIMD(test, height, width);
    display_hexa_vbits_matrix(res, height, width);
    printf("\n\n");*/

    /*size_t width = 679;
    size_t height = 5;
    uint8 **m = ui8matrix(0, height-1, 0, width-1);
    vbits** test;

    for(int i = 0; i < height; i++){
      for(int j = 32; j < 320; j++){
        m[i][j] = (uint8)255;
      }
      for(int j = 320; j < 345; j++){
        m[i][j] = (uint8)0;
      }
      for(int j = 345; j < 645; j++){
        m[i][j] = (uint8)255;
      }
    }
    test = convert_to_binary(m, height, width);
    vbits** v_res = ouverture_SIMD(test, height, width);
    vbits** v_ref = dilatation_3x3_SIMD(erosion_3x3_SIMD(test, height, width), height, width);
    display_ui8matrix(convert_from_binary(v_res, height, width), 0, height-1, 0, width-1, "%d\t", NULL);
    //display_ui8matrix(convert_from_binary(v_ref, height, width), 0, height-1, 0, width-1, "%d\t", NULL);*/

  return 0;
}
