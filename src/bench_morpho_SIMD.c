#include "bench_morpho_SIMD.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;

void bench_erosion_SIMD_naif(void) {

  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix(&img_u8, nrl, nrh, ncl, nch, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** img_ero;
  CHRONO(img_ero = erosion_3x3_SIMD_naif(img_bin, height, width), cycles);
  printf("erosion 3x3 SIMD naif: %.0f cycles\n", cycles);

  return;
}
void bench_dilatation_SIMD_naif(void) {

  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix(&img_u8, nrl, nrh, ncl, nch, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** img_dil;
  CHRONO(img_dil = dilatation_3x3_SIMD_naif(img_bin, height, width), cycles);
  printf("dilatation 3x3 SIMD naif: %.0f cycles\n", cycles);

  return;
}
void bench_ouverture_SIMD_naif(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = ouverture_SIMD_naif(img_bin, height, width), cycles);
  printf("ouverture SIMD naive: %.0f cycles\n", cycles);

  return;
}
void bench_fermeture_naif(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = fermeture_SIMD_naif(img_bin, height, width), cycles);
  printf("fermeture SIMD naive: %.0f cycles\n", cycles);

  return;
}
void bench_chaine_naif(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = chaine_complete_SIMD_naif(img_bin, height, width), cycles);
  printf("chaine complete SIMD naive : %.0f cycles\n", cycles);

  return;
}

void bench_erosion_SIMD_opti(void) {

  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix(&img_u8, nrl, nrh, ncl, nch, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** img_ero;
  CHRONO(img_ero = erosion_3x3_SIMD_opti(img_bin, height, width), cycles);
  printf("erosion 3x3 SIMD opti: %.0f cycles\n", cycles);

  return;
}
void bench_dilatation_SIMD_opti(void) {

  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix(&img_u8, nrl, nrh, ncl, nch, 0);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** img_dil;

  CHRONO(img_dil = dilatation_3x3_SIMD_opti(img_bin, height, width), cycles);
  printf("dilatation 3x3 SIMD opti: %.0f cycles\n", cycles);

  return;
}
void bench_ouverture_opti_SIMD(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = ouverture_opti_SIMD(img_bin, height, width), cycles);
  printf("ouverture SIMD opti: %.0f cycles\n", cycles);

  return;
}
void bench_fermeture_opti_SIMD(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = fermeture_opti_SIMD(img_bin, height, width), cycles);
  printf("fermeture SIMD opti: %.0f cycles\n", cycles);

  return;
}
void bench_chaine_opti(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = chaine_complete_opti_SIMD(img_bin, height, width), cycles);
  printf("chaine complete SIMD opti: %.0f cycles\n", cycles);

  return;
}

void bench_ouverture_fusion_SIMD(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = ouverture_fusion_SIMD(img_bin, height, width), cycles);
  printf("ouverture fusion SIMD : %.0f cycles\n", cycles);

  return;
}
void bench_fermeture_fusion_SIMD(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = fermeture_fusion_SIMD(img_bin, height, width), cycles);
  printf("fermeture fusion SIMD : %.0f cycles\n", cycles);

  return;
}
void bench_chaine_fusion(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img_u8 = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img_u8, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  vbits ** img_bin = convert_to_binary(img_u8, height, width);

  vbits ** res;

  CHRONO(res = chaine_complete_fusion_SIMD(img_bin, height, width), cycles);
  printf("chaine complete fusion SIMD : %.0f cycles\n", cycles);

}

void bench_morpho_SIMD(void) {

  printf("BENCH MORPHO SIMD\n");
  printf("--------------------\n");
  bench_erosion_SIMD_naif();
  bench_dilatation_SIMD_naif();
  bench_ouverture_SIMD_naif();
  bench_fermeture_naif();
  bench_chaine_naif();
  printf("---\n");
  printf("---\n");
  bench_erosion_SIMD_opti();
  bench_dilatation_SIMD_opti();
  bench_ouverture_opti_SIMD();
  bench_fermeture_opti_SIMD();
  bench_chaine_opti();
  printf("---\n");
  printf("---\n");
  bench_ouverture_fusion_SIMD();
  bench_fermeture_fusion_SIMD();
  bench_chaine_fusion();
  printf("====================\n");
  return;
}
