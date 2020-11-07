#include "test_morpho.h"

void test_erosion() {

  long nrl = 0;
  long nrh = 3;
  long ncl = 0;
  long nch = 5;

  uint8 ** m = ui8matrix(nrl, nrh, ncl, nch);
  display_ui8matrix(m, nrl, nrh, ncl, nch, "%hhu", "matrice originale");

  free_ui8matrix(m, nrl, nrh, ncl, nch);

  return
}
