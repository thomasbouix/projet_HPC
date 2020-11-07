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
  free_ui8matrix(m, nrl, nrh, ncl, nch);

  return;
}
