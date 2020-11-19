#include "morpho.h"

// AND logique sur un voisinage de taille kernel_size
uint8** erosion(uint8** img_with_padding, int height, int width, int kernel_size){
  kernel_size /= 2;
  int res = 255;
  //printf("kernel_size = %d\n\n", kernel_size);
  uint8** m = ui8matrix(0, height-1, 0, width-1);

  for(int i = 0; i < height; i++){
    //printf("i=%d\t", i);
    for(int j = 0; j < width; j++){
      res = 255;
      //printf("j=%d\t", j);
      for(int k = i-kernel_size; k <= i+kernel_size; k++){
        //printf("k=%d\t", k);
        for(int l = j-kernel_size; l <= j+kernel_size; l++){
          //printf("l=%d(%d)\t", l, img_with_padding[k][l]);
          res &= img_with_padding[k][l];
          if(res == 0)
            break;  // Sortie de la boucle K
        }
      }
      //printf("RES");
      m[i][j] = res;
    }
    //printf("\n\n");
  }
  return m;
}

// OR logique sur un voisinage de taille kernel_size
uint8** dilatation(uint8** img_with_padding, int height, int width, int kernel_size){
  kernel_size /= 2;
  int res = 0;
  //printf("kernel_size = %d\n\n", kernel_size);
  uint8** m = ui8matrix(0, height-1, 0, width-1);

  for(int i = 0; i < height; i++){
    //printf("i=%d\t", i);
    for(int j = 0; j < width; j++){
      res = 0;
      //printf("j=%d\t", j);
      for(int k = i-kernel_size; k <= i+kernel_size; k++){
        //printf("k=%d\t", k);
        for(int l = j-kernel_size; l <= j+kernel_size; l++){
          //printf("l=%d(%d)\t", l, img_with_padding[k][l]);
          res |= img_with_padding[k][l];
          if(res >= 1)
            break;  // Sortie de la boucle K
        }
      }
      //printf("RES");
      m[i][j] = res;
    }
    //printf("\n\n");
  }
  return m;
}

void compute_erosion(char* basePath, int kernel_size, int save)
{
  CHECK_ERROR(system("mkdir -p output/erosion"));
  int padding = kernel_size/2;
  char buff[35];
  int nrl, nrh, ncl, nch;
  uint8** output = NULL;
  uint8** img = NULL;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_padding_ui8matrix(buff, &nrl, &nrh, &ncl, &nch, padding);
    output = erosion(img, nrh-padding+1, nch-padding+1, kernel_size);
    if(save){
      sprintf(buff, "output/erosion/ero3%.3d.pgm", i);
      SavePGM_ui8matrix(output, nrl+padding, nrh-padding, ncl+padding, nch-padding, buff);
    }
    free_padding_ui8matrix(img, nrl, nrh, ncl, nch, padding);
    free_ui8matrix(output, nrl+padding, nrh-padding, ncl+padding, nch-padding);
  }
}

void compute_dilatation(char* basePath, int kernel_size, int save)
{
  CHECK_ERROR(system("mkdir -p output/dilatation"));
  int padding = kernel_size/2;
  char buff[35];
  int nrl, nrh, ncl, nch;
  uint8** output = NULL;
  uint8** img = NULL;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_padding_ui8matrix(buff, &nrl, &nrh, &ncl, &nch, padding);
    output = dilatation(img, nrh-padding+1, nch-padding+1, kernel_size);
    if(save){
      sprintf(buff, "output/dilatation/dil3%.3d.pgm", i);
      SavePGM_ui8matrix(output, nrl+padding, nrh-padding, ncl+padding, nch-padding, buff);
    }
    free_padding_ui8matrix(img, nrl, nrh, ncl, nch, padding);
    free_ui8matrix(output, nrl+padding, nrh-padding, ncl+padding, nch-padding);
  }
}

uint8** ouverture(uint8** img_with_padding, int height, int width, int kernel_size) {
  int border = 1;
  uint8 ** ero = erosion(img_with_padding, height, width, kernel_size);
  uint8 ** ero_borders = add_borders(ero, height, width, border);
  free_ui8matrix(ero, 0, height-1, 0, width-1);
  return dilatation(ero_borders, height, width, kernel_size);
}
