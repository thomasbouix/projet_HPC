#include "test_morpho_SIMD.h"

void test_erosion_SIMD(void) {

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

  vbits** img_bin = convert_to_binary(m, height, width);
  printf("\nlosange plein binaire\n");
  display_hexa_vbits_matrix(img_bin, height, width);

  printf("losange erosion binaire\n");
  vbits** img_ero = erosion_3x3(img_bin, height, width);
  display_hexa_vbits_matrix(img_ero, height, width);
}

void test_dilatation_SIMD(void) {
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

  vbits** img_bin = convert_to_binary(m, height, width);
  printf("\nlosange creux binaire\n");
  display_hexa_vbits_matrix(img_bin, height, width);

  printf("losange dilatation binaire\n");
  vbits** img_dil = dilatation_3x3(img_bin, height, width);
  display_hexa_vbits_matrix(img_dil, height, width);

}

void test_morpho_SIMD(void) {

  printf("TEST_MORPHO SIMD\n");
  printf("--------------------\n");
  test_erosion_SIMD();
  printf("===\n");
  test_dilatation_SIMD();
  printf("====================\n");

  return;
}
