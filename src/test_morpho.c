#include "test_morpho.h"

void test_erosion() {

  long nrl = 0;
  long nrh = 4;
  long ncl = 0;
  long nch = 4;

  uint8 ** m = ui8matrix(nrl, nrh, ncl, nch);
  m[0][2] = 1;
  m[1][1] = 1;
  m[1][3] = 1;
  m[2][0] = 1;
  m[2][4] = 1;
  m[3][1] = 1;
  m[3][3] = 1;
  m[4][2] = 1;

  display_ui8matrix(m, nrl, nrh, ncl, nch, "%hhu", "losange");
  convert_coding(m, nrl, nrh, ncl, nch, 1, 255);

  SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, "losange");

  int height = nrh-nrl+1;
  int width = nch-ncl+1;
  int border = 1;
  uint8** m_with_borders = add_borders(m, height, width, border);
  free_ui8matrix(m, nrl, nrh, ncl, nch);

  uint8** m_ero = erosion(m_with_borders, height, width, 3);
  printf("\n\n");
  display_ui8matrix(m_ero, nrl, nrh, ncl, nch, "%hhu", "losange_erosion");
  SavePGM_ui8matrix(m_ero, nrl, nrh, ncl, nch, "losange_erosion");

  free_ui8matrix(m_ero, nrl, nrh, ncl, nch);
  free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);

  return;
}
