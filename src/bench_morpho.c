#include "bench_morpho.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;

void bench_erosion(void) {

  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix(&img, nrl, nrh, ncl, nch, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;
  int kernel_size = 3;

  uint8** img_with_borders = add_borders(img, height, width, border);
  free_ui8matrix(img, nrl, nrh, ncl, nch);

  uint8 ** img_ero;

  CHRONO(img_ero = erosion_3x3(img_with_borders, height, width), cycles);
  printf("erosion 3x3 : %.0f cycles\n", cycles);

}

void bench_dilatation(void) {

  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix(&img, nrl, nrh, ncl, nch, 0);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;
  int kernel_size = 3;

  uint8** img_with_borders = add_borders(img, height, width, border);
  free_ui8matrix(img, nrl, nrh, ncl, nch);

  uint8 ** img_ero;

  CHRONO(img_ero = dilatation_3x3(img_with_borders, height, width), cycles);
  printf("dilatation 3x3 : %.0f cycles\n", cycles);
}

void bench_ouverture(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;

  uint8** img_with_borders = add_borders(img, height, width, border);
  uint8 ** res;
  CHRONO(res = ouverture_3x3(img_with_borders, height, width), cycles);
  printf("ouverture 3x3 : %.0f cycles\n", cycles);

  free_padding_ui8matrix(img_with_borders, -1, 240, -1, 320, 1);
  free_ui8matrix(img, nrl, nrh, ncl, nch);
  free_ui8matrix(res, nrl, nrh, ncl, nch);

  return;
}

void bench_fermeture(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;

  uint8** img_with_borders = add_borders(img, height, width, border);
  uint8 ** res;
  CHRONO(res = fermeture_3x3(img_with_borders, height, width), cycles);
  printf("fermeture 3x3 : %.0f cycles\n", cycles);

  free_padding_ui8matrix(img_with_borders, -1, 240, -1, 320, 1);
  free_ui8matrix(img, nrl, nrh, ncl, nch);
  free_ui8matrix(res, nrl, nrh, ncl, nch);

  return;
}

void bench_chaine_complete(void) {
  long nrl = 0;
  long nrh = 239;
  long ncl = 0;
  long nch = 319;
  uint8** img = ui8matrix(nrl, nrh, ncl, nch);
  set_ui8matrix_0255(&img, nrl, nrh, ncl, nch);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;

  uint8** img_with_borders = add_borders(img, height, width, border);
  uint8 ** res;
  CHRONO(res = chaine_complete_3x3(img_with_borders, height, width), cycles);
  printf("chaine complete 3x3 : %.0f cycles\n", cycles);

  free_padding_ui8matrix(img_with_borders, -1, 240, -1, 320, 1);
  free_ui8matrix(img, nrl, nrh, ncl, nch);
  free_ui8matrix(res, nrl, nrh, ncl, nch);

  return;
}

void bench_morpho(void) {

  printf("BENCH_MORPHO\n");
  printf("--------------------\n");
  bench_erosion();
  printf("---\n");
  bench_dilatation();
  printf("---\n");
  bench_ouverture();
  printf("---\n");
  bench_fermeture();
  printf("---\n");
  bench_chaine_complete();
  printf("====================\n");

  return;
}
