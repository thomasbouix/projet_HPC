#include "bench_mouvement.h"

void mesure_routine(void) {

  int nrl, nrh, ncl, nch;

  uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** img1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);

  clock_t begin0 = clock();
  uint8** fd = routine_FrameDifference(img1, img0, nrl, nrh, ncl, nch, 50);
  clock_t end0 = clock();

  printf("routine_fd : %ld cycles \n", (end0 - begin0)*1000);

  free_ui8matrix(img0, nrl, nrh, ncl, nch);
  free_ui8matrix(img1, nrl, nrh, ncl, nch);
  free_ui8matrix(fd, nrl, nrh, ncl, nch);

  return;
}

void mesure_compute_fd(void) {
  clock_t begin1 = clock();
  compute_fd_all_steps(50, 0);
  clock_t end1 = clock();
  printf("compute_fd : %ld ms\n", (end1 - begin1)*1000 / CLOCKS_PER_SEC);
}

void bench_mouvement(void) {

  printf("BENCH_MOUVEMENT\n");
  printf("====================\n");
  mesure_routine();
  mesure_compute_fd();
  printf("====================\n");
  return;
}
