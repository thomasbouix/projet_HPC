#include "bench_mouvement_SIMD.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;

void mesure_routine_FD_SIMD(void) {

  int nrl, nrh, ncl, nch;

  uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** img1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** fd_simd;

  CHRONO(fd_simd = routine_FrameDifference_SIMD(img1, img0, nrl, nrh, ncl, nch, 50), cycles);
  printf("routine_FrameDifference_SIMD : %.0f cycles\n", cycles);

  free_ui8matrix(img0, nrl, nrh, ncl, nch);
  free_ui8matrix(img1, nrl, nrh, ncl, nch);
  free_ui8matrix(fd_simd, nrl, nrh, ncl, nch);

  return;
}

void mesure_all_fd_SIMD(void) {
  CHRONO(compute_fd_SIMD(50, 0), cycles);
  printf("compute_all_fd_SIMD : %.0f cycles\n", cycles);

}

void bench_mouvement_SIMD(void) {

  printf("====================\n");
  printf("BENCH_MOUVEMENT_SIMD\n");
  mesure_routine_FD_SIMD();
  mesure_all_fd_SIMD();
  printf("====================\n");
  return;
}
