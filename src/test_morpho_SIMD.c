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
  // printf("m[0][0] = %d\n", get_bit(img_bin, 0, 0));



  printf("GET_BIT : \n");
  for (int i=0; i<=nrh; i++) {
    for (int j=0; j<=nch; j++) {
      printf("%d", get_bit(img_bin, i, j));
    }
    printf("\n");
  }


  // printf("img_bin[0][3] = %d\n", get_bit(img_bin, 0, 3));
  // printf("\nlosange plein binaire\n");
  // display_hexa_vbits_matrix(img_bin, height, width);
  //
  // printf("losange erosion binaire\n");
  // vbits** img_ero = erosion_3x3_SIMD_opti(img_bin, height, width);
  // display_hexa_vbits_matrix(img_ero, height, width);
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
  vbits** img_dil = dilatation_3x3_SIMD_opti(img_bin, height, width);
  display_hexa_vbits_matrix(img_dil, height, width);

  return;
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 0 0
*/
/*
void test_unitaire_SIMD0(void){
  vbits **erosion_result, **dilatation_result;
  vbits ** m = (vbits**)vui32matrix(0, 2, 0, 2);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != 0){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != 0){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}
*/
void test_morpho_SIMD(void) {

  printf("TEST_MORPHO SIMD\n");
  printf("--------------------\n");
  test_erosion_SIMD();
  printf("====================\n");

  return;
}
