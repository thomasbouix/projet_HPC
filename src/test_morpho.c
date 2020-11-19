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
          ERROR("ero_losange_plein");
        }
      }
      else {
        if (m_ero[i][j] != 0) {
          ERROR("ero_losange_plein");
        }
      }
    }
  }
  SUCCESS("ero_losange_plein");

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
  display_ui8matrix(m, nrl, nrh, ncl, nch, "%hhu", "bordure");


  // uint8** m_with_borders = add_borders(m, height, width, border);
  // free_ui8matrix(m, nrl, nrh, ncl, nch);
  //
  // uint8** m_dil = dilatation(m_with_borders, height, width, 3);
  //
  // convert_coding(m_dil, nrl, nrh, ncl, nch, 255, 1);
  //
  // for (int i=nrl; i<=nrh; i++) {
  //   for (int j=ncl; j<=nch; j++) {
  //     if(i==2 && j==3) {
  //       if (m_ero[i][j] != 1) {
  //         ERROR("ero_losange_plein");
  //       }
  //     }
  //     else {
  //       if (m_ero[i][j] != 0) {
  //         ERROR("ero_losange_plein");
  //       }
  //     }
  //   }
  // }
  SUCCESS("dil_bordure");

  // free_ui8matrix(m_dil, nrl, nrh, ncl, nch);
  // free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);

  return;
}

void test_morpho(void) {

  printf("TEST_MORPHO\n");
  printf("--------------------\n");
  test_ero_5x7_losange_plein();
  printf("===\n");
  test_dil_bordure(10, 10);
  printf("====================\n");

  return;
}
