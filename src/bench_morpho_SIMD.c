#include "bench_morpho_SIMD.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;

void bench_erosion_SIMD(void) {

  int nrl, nrh, ncl, nch;
  uint8** img_u8 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** img_ero;
  CHRONO(img_ero = erosion_3x3_SIMD(img_bin, height, width), cycles);
  printf("erosion 3x3 SIMD : %.0f cycles\n", cycles);

  return;
}

/*
void bench_dilatation_SIMD(void) {

  int nrl, nrh, ncl, nch;
  uint8** img_u8 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** img_dil;

  CHRONO(img_dil = dilatation_3x3_SIMD(img_bin, height, width), cycles);
  printf("dilatation 3x3 SIMD : %.0f cycles\n", cycles);

  return;

}
*/

void bench_morpho_SIMD(void) {

  printf("BENCH_MORPHO SIMD\n");
  printf("--------------------\n");
  bench_erosion_SIMD();
  printf("---\n");
  // bench_dilatation_SIMD();
  printf("====================\n");
  return;
}
