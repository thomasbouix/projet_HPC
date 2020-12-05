#include "bench_chaine_complete.h"

// chronometrie
static int iter, niter = 4;
static int run, nrun = 5;
static double t0, t1, dt, tmin, t;
static double cycles;


void bench_chaine_complete_csv(int taille_min, int taille_max, int pas)
{
  srand(time(NULL));
  FILE *res_csv = fopen("resultats_morpho_chaine_complete.csv", "w");
  fprintf(res_csv, "Taille image;Nombre de points;Cycles scalaire;Cycles SIMD naif;Cycles SIMD opti;Cycles SIMD fusion;");
  fclose(res_csv);
  uint8** generated_img;

  for(int i = taille_min; i <= taille_max; i += pas){
    printf("Taille %dx%d\n",i,i);
    res_csv = fopen("resultats_morpho_chaine_complete.csv", "a");
    fprintf(res_csv, "\n%dx%d;%d;",i,i,i*i);
    fclose(res_csv);
    generated_img = ui8matrix(0, i-1, 0, i-1);
    for(int j = 0; j < i; j++){
      for(int k = 0; k < i; k++){
        generated_img[j][k] = rand() % 2;
      }
    }
    convert_coding(generated_img, 0, i-1, 0, i-1, 1, 255);
    bench_chaine_complete_scalaire(generated_img, i);
    bench_chaine_complete_SIMD_naif(generated_img, i);
    bench_chaine_complete_SIMD_opti(generated_img, i);
    bench_chaine_complete_SIMD_fusion(generated_img, i);
    free_ui8matrix(generated_img, 0, i-1, 0, i-1);
  }
}


void bench_chaine_complete_scalaire(uint8** img, int taille)
{
  int border = 1;
  uint8** img_with_borders = add_borders(img, taille, taille, border);
  FILE *res_csv = fopen("resultats_morpho_chaine_complete.csv", "a");

  CHRONO(chaine_complete_3x3(img_with_borders, taille, taille), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);

  free_padding_ui8matrix(img_with_borders, -1, taille, -1, taille, border);
}

void bench_chaine_complete_SIMD_naif(uint8** img, int taille)
{
  vbits **img_bin = convert_to_binary(img, taille, taille);
  FILE *res_csv = fopen("resultats_morpho_chaine_complete.csv", "a");

  CHRONO(chaine_complete_SIMD_naif(img_bin, taille, taille), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);

  free_vbitsmatrix(img_bin, taille, taille);
}

void bench_chaine_complete_SIMD_opti(uint8** img, int taille)
{
  vbits ** img_bin = convert_to_binary(img, taille, taille);
  FILE *res_csv = fopen("resultats_morpho_chaine_complete.csv", "a");

  CHRONO(chaine_complete_opti_SIMD(img_bin, taille, taille), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);

  free_vbitsmatrix(img_bin, taille, taille);
}

void bench_chaine_complete_SIMD_fusion(uint8** img, int taille)
{
  vbits ** img_bin = convert_to_binary(img, taille, taille);
  FILE *res_csv = fopen("resultats_morpho_chaine_complete.csv", "a");

  CHRONO(chaine_complete_fusion_SIMD(img_bin, taille, taille), cycles);
  fprintf(res_csv, "%.0f;", cycles);
  fclose(res_csv);

  free_vbitsmatrix(img_bin, taille, taille);
}
