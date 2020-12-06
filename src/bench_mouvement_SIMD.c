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

void mesure_SigmaDelta_1step_SIMD(void) {
  int si0, si1, sj0, sj1;
  int vi0, vi1, vj0, vj1;

  vuint8** I1 = LoadPGM_vui8matrix("car3/car_3001.pgm", &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);
  vuint8** M0;
  vuint8** V0;
  vuint8** M1;
  vuint8** V1;
  vuint8** E1;

  SigmaDelta_step0_SIMD(&M0, &V0, &vi0, &vi1, &vj0, &vj1);

  CHRONO(E1 = SigmaDelta_1step_SIMD(M0, &M1, V0, &V1, I1, vi0, vi1, vj0, vj1), cycles);
  printf("SigmaDelta_1step_SIMD : %.0f cycles\n", cycles);

  free_vui8matrix(I1, vi0, vi1, vj0, vj1);
  free_vui8matrix(M0, vi0, vi1, vj0, vj1);
  free_vui8matrix(V0, vi0, vi1, vj0, vj1);
  free_vui8matrix(M1, vi0, vi1, vj0, vj1);
  free_vui8matrix(V1, vi0, vi1, vj0, vj1);
  free_vui8matrix(E1, vi0, vi1, vj0, vj1);

  return;
}

void bench_mouvement_SIMD(void) {

  printf("BENCH MOUVEMENT SIMD\n");
  printf("--------------------\n");
  mesure_routine_FD_SIMD();
  printf("---\n");
  mesure_SigmaDelta_1step_SIMD();
  printf("====================\n");
  return;
}


void bench_mouvement_csv(int taille_min, int taille_max, int pas)
{
  srand(time(NULL));
  FILE *res_csv = fopen("resultats_mouvement.csv", "w");
  fprintf(res_csv, "Taille image;Nombre de points;Cycles FD scalaire;Cycles FD SIMD;Cycles SD scalaire;Cycles SD SIMD;");
  fclose(res_csv);
  uint8** generated_img_0;
  uint8** generated_img_1;

  for(int i = taille_min; i <= taille_max; i += pas){
    printf("Taille %dx%d\n",i,i);
    res_csv = fopen("resultats_mouvement.csv", "a");
    fprintf(res_csv, "\n%dx%d;%d;",i,i,i*i);
    fclose(res_csv);
    generated_img_0 = ui8matrix(0, i-1, 0, i-1);
    generated_img_1 = ui8matrix(0, i-1, 0, i-1);
    for(int j = 0; j < i; j++){
      for(int k = 0; k < i; k++){
        generated_img_0[j][k] = rand() % 256;
        generated_img_1[j][k] = rand() % 256;
      }
    }
    bench_FD_scalaire(generated_img_0, generated_img_1, i);
    bench_FD_SIMD(generated_img_0, generated_img_1, i);
    bench_SD_scalaire(generated_img_0, generated_img_1, i);
    bench_SD_SIMD(generated_img_0, generated_img_1, i);
    free_ui8matrix(generated_img_0, 0, i-1, 0, i-1);
    free_ui8matrix(generated_img_1, 0, i-1, 0, i-1);
  }
}

void bench_FD_scalaire(uint8** img0, uint8** img1, int taille)
{
  FILE *res_csv = fopen("resultats_mouvement.csv", "a");

  CHRONO(routine_FrameDifference(img1, img0, 0, taille-1, 0, taille-1, 50), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);
}

void bench_FD_SIMD(uint8** img0, uint8** img1, int taille)
{
  int vi0, vi1, vj0, vj1;
  FILE *res_csv = fopen("resultats_mouvement.csv", "a");
  s2v(0, taille-1, 0, taille-1, 16, &vi0, &vi1, &vj0, &vj1);

  CHRONO(routine_FrameDifference_SIMD((vuint8**)img1, (vuint8**)img0, vi0, vi1, vj0, vj1, 50), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);
}

void bench_SD_scalaire(uint8** M_0, uint8** I_1, int taille)
{
  FILE *res_csv = fopen("resultats_mouvement.csv", "a");

  uint8** V_0 = ui8matrix(0, taille-1, 0, taille-1);
  for(int i = 0; i < taille; i++){
    for(int j = 0; j < taille; j++){
      V_0[i][j] = V_MIN;
    }
  }

  uint8 **M_1, **V_1;

  CHRONO(SigmaDelta_1step(M_0, &M_1, V_0, &V_1, I_1, 0, taille-1, 0, taille-1), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);
  free_ui8matrix(V_0, 0, taille-1, 0, taille-1);
}

void bench_SD_SIMD(uint8** M_0, uint8** I_1, int taille)
{
  int vi0, vi1, vj0, vj1;
  FILE *res_csv = fopen("resultats_mouvement.csv", "a");
  s2v(0, taille-1, 0, taille-1, 16, &vi0, &vi1, &vj0, &vj1);

  vuint8** M1;
  vuint8** V1;

  vuint8 **V_0 = vui8matrix(vi0, vi1, vj0, vj1);
  vuint8 initial_V = init_vuint8(V_MIN);

  for(int i = vi0; i <= vi1; i++){
    for(int j = vj0; j <= vj1; j++){
      vec_store(&V_0[i][j], initial_V);
    }
  }

  CHRONO(SigmaDelta_1step_SIMD((vuint8**)M_0, &M1, V_0, &V1, (vuint8**)I_1, vi0, vi1, vj0, vj1), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);
  
  free_vui8matrix(V_0, vi0, vi1, vj0, vj1);
}
