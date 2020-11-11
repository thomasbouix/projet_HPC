#include "bench_mouvement_SIMD.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;

void mesure_routine_FD_SIMD(void) {

  int si0, si1, sj0, sj1;
  int vi0, vi1, vj0, vj1;

  vuint8** img0 = LoadPGM_vui8matrix("car3/car_3000.pgm", &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);
  vuint8** img1 = LoadPGM_vui8matrix("car3/car_3001.pgm", &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);
  vuint8** fd_simd = NULL;

  // execute (nrun * niter) fois routine_fd_SIMD sans nettoyer la sortie
  CHRONO(fd_simd = routine_FrameDifference_SIMD(img1, img0, vi0, vi1, vj0, vj1, 50), cycles);
  printf("routine_FrameDifference_SIMD : %.0f cycles\n", cycles);

  free_vui8matrix(img0, vi0, vi1, vj0, vj1);
  free_vui8matrix(img1, vi0, vi1, vj0, vj1);
  free_vui8matrix(fd_simd, vi0, vi1, vj0, vj1);

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
