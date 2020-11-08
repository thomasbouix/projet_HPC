#include "bench_mouvement_SIMD.h"

void mesure_routine_SIMD(void) {

  int nrl, nrh, ncl, nch;

  uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** img1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);

  clock_t begin0 = clock();
  uint8** fd_simd = routine_FrameDifference_SIMD(img1, img0, nrl, nrh, ncl, nch, 50);
  clock_t end0 = clock();

  printf("routine_fd_SIMD : %ld cycles \n", (end0 - begin0)*1000);

  free_ui8matrix(img0, nrl, nrh, ncl, nch);
  free_ui8matrix(img1, nrl, nrh, ncl, nch);
  free_ui8matrix(fd_simd, nrl, nrh, ncl, nch);

  return;
}

void mesure_compute_fd_SIMD(void) {
  clock_t begin1 = clock();
  compute_fd_SIMD(50, 0);
  clock_t end1 = clock();
  printf("compute_fd_SIMD : %ld ms\n", (end1 - begin1)*1000 / CLOCKS_PER_SEC);
}

void bench_mouvement_SIMD(void) {

  printf("BENCH_MOUVEMENT_SIMD\n");
  printf("====================\n");
  mesure_routine_SIMD();
  mesure_compute_fd_SIMD();
  printf("====================\n");
  return;
}
