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

  CHRONO(img_ero = erosion(img_with_borders, height, width, kernel_size), cycles);
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

  CHRONO(img_ero = dilatation(img_with_borders, height, width, kernel_size), cycles);
  printf("dilatation 3x3 : %.0f cycles\n", cycles);
}

void bench_morpho(void) {

  printf("BENCH_MORPHO\n");
  printf("--------------------\n");
  bench_erosion();
  printf("---\n");
  bench_dilatation();
  printf("====================\n");

  return;
}
