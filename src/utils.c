#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "utils.h"
#include "morpho.h"
#include "vnrdef.h"
#include "vnrutil.h"

// Perform convertion between logical and binary convert_coding
// if from = 255 and to = 1 --> conversion from logical to binary coding
// if from = 1 and to = 255 --> conversion from binary to logical coding
void convert_coding(uint8** img, int nrl, int nrh, int ncl, int nch, int from, int to)
{
  for(int i = nrl; i <= nrh; i++){
    for(int j = ncl; j <= nch; j++){
      if(img[i][j] == from)
        img[i][j] = to;
    }
  }
}

uint8** LoadPGM_padding_ui8matrix(char *filename, int* nrl, int* nrh, int* ncl, int* nch, int padding)
{
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

void free_padding_ui8matrix(uint8** img_with_padding, int nrl, int nrh, int ncl, int nch, int padding)
{
  img_with_padding[-padding] -= padding*(nch-ncl+1);
  free_ui8matrix(img_with_padding, nrl, nrh, ncl, nch);
}

vuint8** LoadPGM_vui8matrix(char *filename, int* si0, int* si1, int* sj0, int* sj1, int* vi0, int* vi1, int* vj0, int* vj1)
{
  int card = 16;
  uint8** img = LoadPGM_ui8matrix(filename, si0, si1, sj0, sj1);
  s2v(*si0, *si1, *sj0, *sj1, card, vi0, vi1, vj0, vj1);

  vuint8** img_vuint8 = vui8matrix_map(*vi0, *vi1, *vj0, *vj1);
  int ncol = (*(vj1) - *(vj0)) + 1;
  img_vuint8 = vui8matrix_map_1D_pitch(img_vuint8, *vi0, *vi1, *vj0, *vj1, img[0], ncol);
  free(img);
  return img_vuint8;
}

uint8** add_borders(uint8 **m, int height, int width, int border)
{
  uint8** m_with_borders = ui8matrix(-border, height+border-1 , -border, width+border-1);
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      m_with_borders[i][j] = m[i][j];
    }
  }

  for(int i = -border; i < 0; i++)
    m_with_borders[i] = m_with_borders[0];

  for(int i = height; i <= height-1+border; i++)
    m_with_borders[i] = m_with_borders[height-1];

  for(int i = 0; i < height; i++){
    for(int j = -border; j < 0; j++)
      m_with_borders[i][j] = m_with_borders[i][0];

    for(int j = width; j <= width-1+border; j++)
      m_with_borders[i][j] = m_with_borders[i][width-1];
  }
  return m_with_borders;
}
