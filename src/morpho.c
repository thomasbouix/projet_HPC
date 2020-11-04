#include "morpho.h"

// OR sur le voisinage
uint8 ** routine_dilatation3(uint8** image, int nrl, int nrh, int ncl, int nch) {

  uint8 ** res = ui8matrix(nrl, nrh, ncl, nch);

  /* --- remplissage des bords --- */
  for (int i=nrl; i<=nrh; i++) res[i][ncl] = image[i][ncl]; // ligne du haut
  for (int i=nrl; i<=nrh; i++) res[i][nch] = image[i][nch]; // ligne du bas
  for (int j=ncl; j<=nch; j++) res[nrl][j] = image[nrl][j]; // colonne de gauche
  for (int j=ncl; j<=nch; j++) res[nrh][j] = image[nrh][j]; // colonne de gauche
  /* ----------------------------- */

  // Parcours de l'image
  for(int i=nrl+1; i < nrh; i++) {
    for(int j=ncl+1; j < nch; j++) {
      // Parcours du voisinage
      for (int x=-1; x<=1; x++) {
        for (int y=-1; y<=1; y++) {
          if (image[i+x][j+y] == 255) {   // On a trouvé un 255
            res[i][j] = 255;
            goto end;
          }
        }
      }
      res[i][j] = 0;                      // On n'a pas trouvé de 255
      end:;
    }
  }

  return res;
}

void compute_all_dilatation3(void) {
  CHECK_ERROR(system("mkdir -p output/morpho"));

  char image_path[40];
  char res_path[40];
  int nrl, nrh, ncl, nch;

  for(int i = 1; i < 200; i++){
    sprintf(image_path, "output/sd/E_t_%.3d.pgm", i);
    uint8 ** image = LoadPGM_ui8matrix(image_path, &nrl, &nrh, &ncl, &nch);

    uint8 ** res = routine_dilatation3(image, nrl, nrh, ncl, nch);
    sprintf(res_path, "output/morpho/D_t_%.3d.pgm", i);
    SavePGM_ui8matrix(res, nrl, nrh, ncl, nch, res_path);
    free_ui8matrix(res, nrl, nrh, ncl, nch);
  }
}
