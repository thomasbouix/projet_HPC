#include "test_morpho.h"

void test_erosion(void) {

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

  display_ui8matrix(m, nrl, nrh, ncl, nch, "%hhu", "losange plein");
  convert_coding(m, nrl, nrh, ncl, nch, 1, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;

  uint8** m_with_borders = add_borders(m, height, width, border);
  free_ui8matrix(m, nrl, nrh, ncl, nch);

  uint8** m_ero = erosion(m_with_borders, height, width, 3);

  convert_coding(m_ero, nrl, nrh, ncl, nch, 255, 1);
  display_ui8matrix(m_ero, nrl, nrh, ncl, nch, "%hhu", "\nlosange_erosion");

  free_ui8matrix(m_ero, nrl, nrh, ncl, nch);
  free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);

  return;
}

void test_dilatation(void) {

  long nrl = 0;
  long nrh = 4;
  long ncl = 0;
  long nch = 6;

  uint8 ** m = ui8matrix(nrl, nrh, ncl, nch);
  zero_ui8matrix(&m, nrl, nrh, ncl, nch);

  m[0][3] = 1;
  m[1][2] = 1; m[1][4] = 1;
  m[2][1] = 1; m[2][5] = 1;
  m[3][2] = 1; m[3][4] = 1;
  m[4][3] = 1;

  display_ui8matrix(m, nrl, nrh, ncl, nch, "%hhu", "losange creux");
  convert_coding(m, nrl, nrh, ncl, nch, 1, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;

  uint8** m_with_borders = add_borders(m, height, width, border);
  free_ui8matrix(m, nrl, nrh, ncl, nch);

  uint8** m_dil = dilatation(m_with_borders, height, width, 3);

  convert_coding(m_dil, nrl, nrh, ncl, nch, 255, 1);
  display_ui8matrix(m_dil, nrl, nrh, ncl, nch, "%hhu", "\nlosange_dilatation");

  free_ui8matrix(m_dil, nrl, nrh, ncl, nch);
  free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);

  return;
}

void test_morpho(void) {

  printf("====================\n");
  printf("TEST_MORPHO\n\n");
  test_erosion();
  printf("===\n");
  test_dilatation();
  printf("====================\n");

  return;
}
