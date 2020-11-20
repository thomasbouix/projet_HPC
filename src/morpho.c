#include "morpho.h"

// AND logique sur un voisinage de taille 3x3
uint8** erosion_3x3(uint8** img_with_padding, int height, int width){
  uint8** m = ui8matrix(0, height-1, 0, width-1);

  /* Pattern
    x0 x1 x2
    x3 x4 x5
    x6 x7 x8
  */
  int x0, x1, x2;
  int x3, x4, x5;
  int x6, x7, x8;

  int and0, and1, and2;

  x0 = img_with_padding[-1][-1]; x1 = img_with_padding[-1][0];
  x3 = img_with_padding[-0][-1]; x4 = img_with_padding[-0][0];

  # pragma omp parallel
  {
    # pragma omp for
    for(int i = 0; i < height; i++){
      x6 = img_with_padding[i+1][-1]; x7 = img_with_padding[i+1][-0];
      and0 = x0 & x3 & x6;
      and1 = x1 & x4 & x7;
      for(int j = 0; j < width; j++){
        x2 = img_with_padding[i-1][j+1];
        x5 = img_with_padding[i-0][j+1];
        x8 = img_with_padding[i+1][j+1];
        and2 = x2 & x5 & x8;
        m[i][j] = and0 & and1 & and2;
        and0 = and1; and1 = and2;
      }
      x0 = x3; x3 = x6;
      x1 = x4; x4 = x7;
    }
  }
  return m;
}

// OR logique sur un voisinage de taille kernel_size
uint8** dilatation_3x3(uint8** img_with_padding, int height, int width){
  uint8** m = ui8matrix(0, height-1, 0, width-1);

  /* Pattern
    x0 x1 x2
    x3 x4 x5
    x6 x7 x8
  */
  int x0, x1, x2;
  int x3, x4, x5;
  int x6, x7, x8;

  int or0, or1, or2;

  x0 = img_with_padding[-1][-1]; x1 = img_with_padding[-1][0];
  x3 = img_with_padding[-0][-1]; x4 = img_with_padding[-0][0];
  for(int i = 0; i < height; i++){
    x6 = img_with_padding[i+1][-1]; x7 = img_with_padding[i+1][-0];
    or0 = x0 | x3 | x6;
    or1 = x1 | x4 | x7;
    for(int j = 0; j < width; j++){
      x2 = img_with_padding[i-1][j+1];
      x5 = img_with_padding[i-0][j+1];
      x8 = img_with_padding[i+1][j+1];
      or2 = x2 | x5 | x8;
      m[i][j] = or0 | or1 | or2;
      or0 = or1; or1 = or2;
    }
    x0 = x3; x3 = x6;
    x1 = x4; x4 = x7;
  }
  return m;
}

void compute_erosion_3x3(char* basePath, int save)
{
  CHECK_ERROR(system("mkdir -p output/erosion"));
  int bord = 1;
  char buff[35];
  int nrl, nrh, ncl, nch;
  uint8** output = NULL;
  uint8** img = NULL;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_padding_ui8matrix(buff, &nrl, &nrh, &ncl, &nch, bord);
    output = erosion_3x3(img, nrh-bord+1, nch-bord+1);
    if(save){
      sprintf(buff, "output/erosion/ero3%.3d.pgm", i);
      SavePGM_ui8matrix(output, nrl+bord, nrh-bord, ncl+bord, nch-bord, buff);
    }
    free_padding_ui8matrix(img, nrl, nrh, ncl, nch, bord);
    free_ui8matrix(output, nrl+bord, nrh-bord, ncl+bord, nch-bord);
  }
}

void compute_dilatation_3x3(char* basePath, int save)
{
  CHECK_ERROR(system("mkdir -p output/dilatation"));
  int bord = 1;
  char buff[35];
  int nrl, nrh, ncl, nch;
  uint8** output = NULL;
  uint8** img = NULL;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_padding_ui8matrix(buff, &nrl, &nrh, &ncl, &nch, bord);
    output = dilatation_3x3(img, nrh-bord+1, nch-bord+1);
    if(save){
      sprintf(buff, "output/dilatation/dil3%.3d.pgm", i);
      SavePGM_ui8matrix(output, nrl+bord, nrh-bord, ncl+bord, nch-bord, buff);
    }
    free_padding_ui8matrix(img, nrl, nrh, ncl, nch, bord);
    free_ui8matrix(output, nrl+bord, nrh-bord, ncl+bord, nch-bord);
  }
}

uint8** ouverture(uint8** img_with_padding, int height, int width, int kernel_size) {
  int border = 1;
  uint8 ** ero = erosion_3x3(img_with_padding, height, width);
  uint8 ** ero_borders = add_borders(ero, height, width, border);
  free_ui8matrix(ero, 0, height-1, 0, width-1);
  return dilatation_3x3(ero_borders, height, width);
}
