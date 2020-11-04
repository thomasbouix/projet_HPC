#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "utils.h"
#include "morpho.h"


uint8** LoadPGM_padding_ui8matrix(char *filename, int* nrl, int* nrh, int* ncl, int* nch, int padding){
  uint8** m = ui8matrix(-padding, IMG_ROWS+padding, -padding, IMG_COLS+padding);
  MLoadPGM_ui8matrix(filename, 0, IMG_ROWS, 0, IMG_COLS, m);
  // On fait pointer les lignes de padding vers les lignes 0 et 239
  for(int i = -padding; i < 0; i++)
    m[i] = m[0];
  for(int i = IMG_ROWS+1; i <= IMG_ROWS+padding; i++)
    m[i] = m[IMG_ROWS];

  // On recopie les valeurs de la périphérie de l'image dans les colonnes de padding
  for(int i = 0; i <= IMG_ROWS; i++){
    for(int j = -padding; j < 0; j++)
      m[i][j] = m[i][0];
    for (int j = IMG_COLS+1; j <= IMG_COLS+padding; j++)
      m[i][j] = m[i][IMG_COLS];
  }
  *nrl = -padding;
  *nrh = IMG_ROWS+padding;
  *ncl = -padding;
  *nch = IMG_COLS+padding;
  return m;
}

// AND logique sur un voisinage de taille kernel_size
uint8** erosion(uint8** img_with_padding, int height, int width, int kernel_size){
  kernel_size /= 2;
  int res = 255;
  //printf("kernel_size = %d\n\n", kernel_size);
  uint8** m = ui8matrix(0, height, 0, width);

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
  uint8** m = ui8matrix(0, height, 0, width);

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

void compute_erosion(char* basePath, int kernel_size){
  CHECK_ERROR(system("mkdir -p output/erosion"));
  int padding = kernel_size/2;
  char buff[35];
  int nrl, nrh, ncl, nch;
  uint8** output = NULL;
  uint8** img = NULL;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_padding_ui8matrix(buff, &nrl, &nrh, &ncl, &nch, padding);
    output = erosion(img, IMG_ROWS+1, IMG_COLS+1, kernel_size);
    sprintf(buff, "output/erosion/ero3%.3d.pgm", i);
    SavePGM_ui8matrix(output, 0, IMG_ROWS, 0, IMG_COLS, buff);
    img[-padding] -= padding*(nch-ncl+1);
    free_ui8matrix(img, nrl, nrh, ncl, nch);
    free_ui8matrix(output, 0, IMG_ROWS, 0, IMG_COLS);
  }
}

void compute_dilatation(char* basePath, int kernel_size){
  CHECK_ERROR(system("mkdir -p output/dilatation"));
  int padding = kernel_size/2;
  char buff[35];
  int nrl, nrh, ncl, nch;
  uint8** output = NULL;
  uint8** img = NULL;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_padding_ui8matrix(buff, &nrl, &nrh, &ncl, &nch, padding);
    output = dilatation(img, IMG_ROWS+1, IMG_COLS+1, kernel_size);
    sprintf(buff, "output/dilatation/dil3%.3d.pgm", i);
    SavePGM_ui8matrix(output, 0, IMG_ROWS, 0, IMG_COLS, buff);
    img[-padding] -= padding*(nch-ncl+1);
    free_ui8matrix(img, nrl, nrh, ncl, nch);
    free_ui8matrix(output, 0, IMG_ROWS, 0, IMG_COLS);
  }
}
