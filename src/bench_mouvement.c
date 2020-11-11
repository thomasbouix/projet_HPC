#include "bench_mouvement.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;

void mesure_routine_fd(void) {

  int nrl, nrh, ncl, nch;

  uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** img1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);
  uint8** fd;

  CHRONO(fd = routine_FrameDifference(img1, img0, nrl, nrh, ncl, nch, 50), cycles);
  printf("routine_FrameDifference : %.0f cycles\n", cycles);

  free_ui8matrix(img0, nrl, nrh, ncl, nch);
  free_ui8matrix(img1, nrl, nrh, ncl, nch);
  free_ui8matrix(fd, nrl, nrh, ncl, nch);

  return;
}

void mesure_all_fd(void) {

  CHRONO(compute_fd_all_steps(50, 0), cycles);
  printf("compute_fd_all_steps : %.0f cycles\n", cycles);

}

void mesure_SigmaDelta_1step(void) {
  int nrl, nrh, ncl, nch;

  uint8 ** M_0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8 ** V_0 = ui8matrix(nrl, nrh, ncl, nch);
  SigmaDelta_step0(&M_0, &V_0, &nrl, &nrh, &ncl, &nch);

  uint8 ** I_1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);
  uint8 ** M_1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8 ** V_1 = ui8matrix(nrl, nrh, ncl, nch);;
  uint8 ** E_1;

  CHRONO(E_1 = SigmaDelta_1step(M_0, &M_1, V_0, &V_1, I_1, nrl, nrh, ncl, nch), cycles);
  printf("SigmaDelta_1step : %.0f cycles \n", cycles);

  free_ui8matrix(M_0, nrl, nrh, ncl, nch);
  free_ui8matrix(V_0, nrl, nrh, ncl, nch);
  free_ui8matrix(M_1, nrl, nrh, ncl, nch);
  free_ui8matrix(V_1, nrl, nrh, ncl, nch);
  free_ui8matrix(I_1, nrl, nrh, ncl, nch);
  free_ui8matrix(E_1, nrl, nrh, ncl, nch);

}

void mesure_SigmaDelta_1step_opti(void) {
  int nrl, nrh, ncl, nch;

  uint8 ** M_0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
  uint8 ** V_0 = ui8matrix(nrl, nrh, ncl, nch);
  SigmaDelta_step0(&M_0, &V_0, &nrl, &nrh, &ncl, &nch);

  uint8 ** I_1 = LoadPGM_ui8matrix("car3/car_3001.pgm", &nrl, &nrh, &ncl, &nch);
  uint8 ** M_1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8 ** V_1 = ui8matrix(nrl, nrh, ncl, nch);;
  uint8 ** E_1;

  CHRONO(E_1 = SigmaDelta_1step_opti(M_0, &M_1, V_0, &V_1, I_1, nrl, nrh, ncl, nch), cycles);
  printf("SigmaDelta_1step_opti : %.0f cycles \n", cycles);

  free_ui8matrix(M_0, nrl, nrh, ncl, nch);
  free_ui8matrix(V_0, nrl, nrh, ncl, nch);
  free_ui8matrix(M_1, nrl, nrh, ncl, nch);
  free_ui8matrix(V_1, nrl, nrh, ncl, nch);
  free_ui8matrix(I_1, nrl, nrh, ncl, nch);
  free_ui8matrix(E_1, nrl, nrh, ncl, nch);

}

void bench_mouvement(void) {

  printf("====================\n");
  printf("BENCH_MOUVEMENT\n");
  mesure_routine_fd();
  mesure_SigmaDelta_1step();
  mesure_SigmaDelta_1step_opti();
  mesure_all_fd();
  printf("====================\n");
  return;
}
