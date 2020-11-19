#include "test_morpho.h"

void test_ero_5x7_losange_plein(void) {

  long nrl = 0;
  long nrh = 4;
  long ncl = 0;
  long nch = 6;

  uint8 ** m = ui8matrix(nrl, nrh, ncl, nch);
  zero_ui8matrix(&m, nrl, nrh, ncl, nch);
  m[0][3] = 1;
  m[1][2] = 1; m[1][3] = 1; m[1][4] = 1;
  m[2][1] = 1; m[2][2] = 1; m[2][3] = 1; m[2][4] = 1; m[2][5] = 1;
  m[3][2] = 1; m[3][3] = 1; m[3][4] = 1;
  m[4][3] = 1;

  // display_ui8matrix(m, nrl, nrh, ncl, nch, "%hhu", "losange plein");
  convert_coding(m, nrl, nrh, ncl, nch, 1, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;

  uint8** m_with_borders = add_borders(m, height, width, border);
  free_ui8matrix(m, nrl, nrh, ncl, nch);

  uint8** m_ero = erosion(m_with_borders, height, width, 3);

  convert_coding(m_ero, nrl, nrh, ncl, nch, 255, 1);
  // display_ui8matrix(m_ero, nrl, nrh, ncl, nch, "%hhu", "\nlosange_erosion");

  for (int i=nrl; i<=nrh; i++) {
    for (int j=ncl; j<=nch; j++) {
      if(i==2 && j==3) {
        if (m_ero[i][j] != 1) {
          ERROR(__func__);
        }
      }
      else {
        if (m_ero[i][j] != 0) {
          ERROR(__func__);
        }
      }
    }
  }
  SUCCESS(__func__);

  free_ui8matrix(m_ero, nrl, nrh, ncl, nch);
  free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);

  return;
}

void test_dil_bordure(int height, int width) {

  long nrl = 0;
  long nrh = height-1;
  long ncl = 0;
  long nch = width-1;
  int border = 1;

  uint8 ** m = ui8matrix(nrl, nrh, ncl, nch);
  zero_ui8matrix(&m, nrl, nrh, ncl, nch);
  set_ui8_bordures(&m, nrl, nrh, ncl, nch, (uint8) 1);

  uint8** m_with_borders = add_borders(m, height, width, border);
  free_ui8matrix(m, nrl, nrh, ncl, nch);

  uint8** m_res = dilatation(m_with_borders, height, width, 3);
  // display_ui8matrix(m_res, nrl, nrh, ncl, nch, "%hhu", "\nm_res");

  // VERIFICATION
  // bordures horizontales exterieures
  for (int j=ncl; j<=nch; j++) {

    if (m_res[nrl][j] != 1) {
      printf("%d\n", m_res[nrl][j]);
      ERROR(__func__);
    }
    if (m_res[nrh][j] != 1) {
      printf("%d\n", m_res[nrh][j]);
      ERROR(__func__);
    }
  }

  // bordures horizontales interieur
  for (int j=ncl+1; j<=nch-1; j++) {
    if (m_res[nrl+1][j] != 1) {
      printf("%d\n", m_res[nrl+1][j]);
      ERROR(__func__);
    }
    if (m_res[nrh-1][j] != 1) {
      printf("%d\n", m_res[nrh-1][j]);
      ERROR(__func__);
    }
  }

  // bordures verticale exterieures
  for (int i=nrl; i<=nrh; i++) {
    if (m_res[i][ncl] != 1) {
      printf("%d\n", m_res[i][ncl]);
      ERROR(__func__);
    }
    if (m_res[i][nch] != 1) {
      printf("%d\n", m_res[i][nch]);
      ERROR(__func__);
    }
  }

  // bordures verticale interieures
  for (int i=nrl+1; i<=nrh-1; i++) {
    if (m_res[i][ncl+1] != 1) {
      printf("%d\n", m_res[i][ncl+1]);
      ERROR(__func__);
    }
    if (m_res[i][nch-1] != 1) {
      printf("%d\n", m_res[i][nch-1]);
      ERROR(__func__);
    }
  }

  // CENTRE = 0
  for (int i=nrl+2; i<=nrh-2; i++) {
    for (int j=ncl+2; j<=nch-2; j++) {
      if (m_res[i][j] != 0) {
        printf("%d\n", m_res[i][j]);
        ERROR(__func__);
      }
    }
  }

  printf("Sucess : ");
  printf(__func__);
  printf(" %dx%d\n", height, width);

  free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);
  free_ui8matrix(m_res, nrl, nrh, ncl, nch);

  return;
}

void test_morpho(void) {

  printf("TEST MORPHO SCALAIRE\n");
  printf("--------------------\n");
  test_ero_5x7_losange_plein();
  test_dil_bordure(3, 3);
  test_dil_bordure(5, 5);
  test_dil_bordure(10, 10);
  test_dil_bordure(100, 100);
  printf("====================\n");

  return;
}
