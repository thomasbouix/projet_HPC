#include "test_morpho_SIMD.h"

// erosion sur losange plein
void test_erosion_losange_SIMD(void) {

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

  convert_coding(m, nrl, nrh, ncl, nch, 1, 255);

  int height = nrh-nrl+1;
  int width = nch-ncl+1;

  vbits** img_bin = convert_to_binary(m, height, width);
  vbits** img_ero = erosion_3x3_SIMD_opti(img_bin, height, width);

  if (get_bit(img_ero, 2, 3) != 1) {
    printf("[2][3]\n");
    ERROR(__func__);
  }

  for (int i=0; i<=nrh; i++) {
    for (int j=0; j<=nch; j++) {
      if( !(i==2 && j==3) ) {
        if (get_bit(img_ero, i, j) != 0) {
          printf("[%d][%d]\n", i, j);
          ERROR(__func__);
        }
      }
    }
  }
  SUCCESS(__func__);

  free_ui8matrix(m, nrl, nrh, ncl, nch);
  free_vbitsmatrix(img_bin, 3, 3);
  free_vbitsmatrix(img_ero, 3, 3);

}
// dilatation sur losange creux
void test_dilatation_losange_SIMD(void) {
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

  convert_coding(m, nrl, nrh, ncl, nch, 1, 255);
  int height = nrh-nrl+1;
  int width = nch-ncl+1;

  vbits** img_bin = convert_to_binary(m, height, width);
  vbits** img_dil = dilatation_3x3_SIMD_opti(img_bin, height, width);

  for (int i=0; i<=nrh; i++) {
    for (int j=0; j<=nch; j++) {
      if (!(i==0 && j==0) && !(i==0 && j==nch) && !(i==nrh && j==0) && !(i==nrh && j==nch)) {
              if (get_bit(img_dil, i, j) != 1) {
                  printf("[%d][%d]\n", i, j);
                  ERROR(__func__);
              }
      }
      else {
              if (get_bit(img_dil, i, j) != 0) {
                  printf("[%d][%d]\n", i, j);
                  ERROR(__func__);
              }
      }
    }
  }

  SUCCESS(__func__);
  free_ui8matrix(m, nrl, nrh, ncl, nch);
  free_vbitsmatrix(img_bin, 3, 3);
  free_vbitsmatrix(img_dil, 3, 3);

  return;
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 0 0
*/
void test_unitaire_SIMD0(void){
  vbits **erosion_result, **dilatation_result;
  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);
  vbits ** m = convert_to_binary(m8, 3, 3);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != 0){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != 0){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 0 0                 0 0 0                         1 1 0
  0 0 0                 0 0 0                         1 1 0
  0 0 0                 0 0 0                         0 0 0
*/
void test_unitaire_SIMD1(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 0, 0, 1);

  set_bit(dilatation_expected_result, 0, 0, 1);
  set_bit(dilatation_expected_result, 0, 1, 1);
  set_bit(dilatation_expected_result, 1, 0, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 1 0                 0 0 0                         1 1 1
  0 0 0                 0 0 0                         1 1 1
  0 0 0                 0 0 0                         0 0 0
*/
void test_unitaire_SIMD2(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 0, 1, 1);

  set_bit(dilatation_expected_result, 0, 0, 1);
  set_bit(dilatation_expected_result, 0, 1, 1);
  set_bit(dilatation_expected_result, 0, 2, 1);
  set_bit(dilatation_expected_result, 1, 0, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 1, 2, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 1                 0 0 0                         0 1 1
  0 0 0                 0 0 0                         0 1 1
  0 0 0                 0 0 0                         0 0 0
*/
void test_unitaire_SIMD3(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 0, 2, 1);

  set_bit(dilatation_expected_result, 0, 1, 1);
  set_bit(dilatation_expected_result, 0, 2, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 1, 2, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         1 1 0
  1 0 0                 0 0 0                         1 1 0
  0 0 0                 0 0 0                         1 1 0
*/
void test_unitaire_SIMD4(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 1, 0, 1);

  set_bit(dilatation_expected_result, 0, 0, 1);
  set_bit(dilatation_expected_result, 0, 1, 1);
  set_bit(dilatation_expected_result, 1, 0, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 2, 0, 1);
  set_bit(dilatation_expected_result, 2, 1, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         1 1 1
  0 1 0                 0 0 0                         1 1 1
  0 0 0                 0 0 0                         1 1 1
*/
void test_unitaire_SIMD5(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 1, 1, 1);

  set_bit(dilatation_expected_result, 0, 0, 1);
  set_bit(dilatation_expected_result, 0, 1, 1);
  set_bit(dilatation_expected_result, 0, 2, 1);
  set_bit(dilatation_expected_result, 1, 0, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 1, 2, 1);
  set_bit(dilatation_expected_result, 2, 0, 1);
  set_bit(dilatation_expected_result, 2, 1, 1);
  set_bit(dilatation_expected_result, 2, 2, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 1 1
  0 0 1                 0 0 0                         0 1 1
  0 0 0                 0 0 0                         0 1 1
*/
void test_unitaire_SIMD6(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 1, 2, 1);

  set_bit(dilatation_expected_result, 0, 1, 1);
  set_bit(dilatation_expected_result, 0, 2, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 1, 2, 1);
  set_bit(dilatation_expected_result, 2, 1, 1);
  set_bit(dilatation_expected_result, 2, 2, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         1 1 0
  1 0 0                 0 0 0                         1 1 0
*/
void test_unitaire_SIMD7(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 2, 0, 1);

  set_bit(dilatation_expected_result, 1, 0, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 2, 0, 1);
  set_bit(dilatation_expected_result, 2, 1, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         1 1 1
  0 1 0                 0 0 0                         1 1 1
*/
void test_unitaire_SIMD8(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 2, 1, 1);

  set_bit(dilatation_expected_result, 1, 0, 1);
  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 1, 2, 1);
  set_bit(dilatation_expected_result, 2, 0, 1);
  set_bit(dilatation_expected_result, 2, 1, 1);
  set_bit(dilatation_expected_result, 2, 2, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 1 1
  0 0 1                 0 0 0                         0 1 1
*/
void test_unitaire_SIMD9(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m8, 0, 2, 0, 2);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 2, 2, 1);


  set_bit(dilatation_expected_result, 1, 1, 1);
  set_bit(dilatation_expected_result, 1, 2, 1);
  set_bit(dilatation_expected_result, 2, 1, 1);
  set_bit(dilatation_expected_result, 2, 2, 1);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/
void test_unitaire_SIMD10(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_result;
  vbits **dilatation_result;

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 1 1                 0 0 1                         1 1 1
  1 1 1                 0 0 1                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/
void test_unitaire_SIMD11(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);


  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 0, 0, 0);

  set_bit(erosion_expected_result, 0, 0, 0);
  set_bit(erosion_expected_result, 0, 1, 0);
  set_bit(erosion_expected_result, 1, 0, 0);
  set_bit(erosion_expected_result, 1, 1, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 0 1                 0 0 0                         1 1 1
  1 1 1                 0 0 0                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/
void test_unitaire_SIMD12(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);


  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 0, 1, 0);

  set_bit(erosion_expected_result, 0, 0, 0);
  set_bit(erosion_expected_result, 0, 1, 0);
  set_bit(erosion_expected_result, 0, 2, 0);
  set_bit(erosion_expected_result, 1, 0, 0);
  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 1, 2, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 0                 1 0 0                         1 1 1
  1 1 1                 1 0 0                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/
void test_unitaire_SIMD13(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 0, 2, 0);

  set_bit(erosion_expected_result, 0, 1, 0);
  set_bit(erosion_expected_result, 0, 2, 0);
  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 1, 2, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 0 0 1                         1 1 1
  0 1 1                 0 0 1                         1 1 1
  1 1 1                 0 0 1                         1 1 1
*/
void test_unitaire_SIMD14(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 1, 0, 0);

  set_bit(erosion_expected_result, 0, 0, 0);
  set_bit(erosion_expected_result, 0, 1, 0);
  set_bit(erosion_expected_result, 1, 0, 0);
  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 2, 0, 0);
  set_bit(erosion_expected_result, 2, 1, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 0 0 0                         1 1 1
  1 0 1                 0 0 0                         1 1 1
  1 1 1                 0 0 0                         1 1 1
*/
void test_unitaire_SIMD15(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 1, 1, 0);

  zero_ui8matrix(&m8, 0, 2, 0, 2);
  vbits **erosion_expected_result = convert_to_binary(m8, 3, 3);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 0 0                         1 1 1
  1 1 0                 1 0 0                         1 1 1
  1 1 1                 1 0 0                         1 1 1
*/
void test_unitaire_SIMD16(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 1, 2, 0);

  set_bit(erosion_expected_result, 0, 1, 0);
  set_bit(erosion_expected_result, 0, 2, 0);
  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 1, 2, 0);
  set_bit(erosion_expected_result, 2, 1, 0);
  set_bit(erosion_expected_result, 2, 2, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 0 0 1                         1 1 1
  0 1 1                 0 0 1                         1 1 1
*/
void test_unitaire_SIMD17(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 2, 0, 0);


  set_bit(erosion_expected_result, 1, 0, 0);
  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 2, 0, 0);
  set_bit(erosion_expected_result, 2, 1, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 0 0 0                         1 1 1
  1 0 1                 0 0 0                         1 1 1
*/
void test_unitaire_SIMD18(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 2, 1, 0);

  set_bit(erosion_expected_result, 1, 0, 0);
  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 1, 2, 0);
  set_bit(erosion_expected_result, 2, 0, 0);
  set_bit(erosion_expected_result, 2, 1, 0);
  set_bit(erosion_expected_result, 2, 2, 0);


  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 1 0 0                         1 1 1
  1 1 0                 1 0 0                         1 1 1
*/
void test_unitaire_SIMD19(void){

  uint8 ** m8 = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m8, 0, 2, 0, 2, 255);

  vbits **erosion_expected_result    = convert_to_binary(m8, 3, 3);
  vbits **dilatation_expected_result = convert_to_binary(m8, 3, 3);
  vbits ** m = convert_to_binary(m8, 3, 3);

  set_bit(m, 2, 2, 0);

  set_bit(erosion_expected_result, 1, 1, 0);
  set_bit(erosion_expected_result, 1, 2, 0);
  set_bit(erosion_expected_result, 2, 1, 0);
  set_bit(erosion_expected_result, 2, 2, 0);

  vbits **erosion_result = erosion_3x3_SIMD_opti(m, 3, 3);
  vbits **dilatation_result = dilatation_3x3_SIMD_opti(m, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(dilatation_result, i, j) != get_bit(dilatation_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_bit(erosion_result, i, j) != get_bit(erosion_expected_result, i, j)){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m8, 0, 2, 0, 2);
  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
}

void test_ouverture_universel(int height, int width) {

  uint8 ** m8 = ui8matrix(0, height-1, 0, width-1);
  zero_ui8matrix(&m8, 0, height-1, 0, width-1);

  vbits ** m = convert_to_binary(m8, height, width);
  vbits ** ouverture_opti   = ouverture_opti_SIMD(m, height, width);
  vbits ** ouverture_fusion = ouverture_fusion_SIMD(m, height, width);

  if (compare_SIMD(ouverture_opti, ouverture_fusion, height, width) == 0) {
    ERROR(__func__);
  }

  free_vbitsmatrix(ouverture_opti, height, width);
  free_vbitsmatrix(ouverture_fusion, height, width);

  // 1 TEST COMPLET PAR PIXEL
  for (int i=0; i<=height-1; i++) {
    for (int j=0; j<=width-1; j++) {

          set_bit(m, i, j, 1);

          ouverture_opti   = ouverture_opti_SIMD(m, height, width);
          ouverture_fusion = ouverture_fusion_SIMD(m, height, width);

          if (compare_SIMD(ouverture_opti, ouverture_fusion, height, width) == 0) {
            ERROR(__func__);
          }

          free_vbitsmatrix(ouverture_opti, height, width);
          free_vbitsmatrix(ouverture_fusion, height, width);
    }
  }

  free_ui8matrix(m8, 0, height-1, 0, width-1);
  free_vbitsmatrix(m, height, width);


  printf("\033[1;32mSucess\033[0m : ");
  printf(__func__);
  printf(" %dx%d\n", height, width);

}

void test_fermeture_universel(int height, int width) {

  uint8 ** m8 = ui8matrix(0, height-1, 0, width-1);
  zero_ui8matrix(&m8, 0, height-1, 0, width-1);

  vbits ** m = convert_to_binary(m8, height, width);
  vbits ** fermeture_opti   = fermeture_opti_SIMD(m, height, width);
  vbits ** fermeture_fusion = fermeture_fusion_SIMD(m, height, width);

  if (compare_SIMD(fermeture_opti, fermeture_fusion, height, width) == 0) {
    ERROR(__func__);
  }

  free_vbitsmatrix(fermeture_opti, height, width);
  free_vbitsmatrix(fermeture_fusion, height, width);

  // 1 TEST COMPLET PAR PIXEL
  for (int i=0; i<=height-1; i++) {
    for (int j=0; j<=width-1; j++) {

          set_bit(m, i, j, 1);

          fermeture_opti   = fermeture_opti_SIMD(m, height, width);
          fermeture_fusion = fermeture_fusion_SIMD(m, height, width);

          if (compare_SIMD(fermeture_opti, fermeture_fusion, height, width) == 0) {
            ERROR(__func__);
          }

          free_vbitsmatrix(fermeture_opti, height, width);
          free_vbitsmatrix(fermeture_fusion, height, width);
    }
  }

  free_ui8matrix(m8, 0, height-1, 0, width-1);
  free_vbitsmatrix(m, height, width);


  printf("\033[1;32mSucess\033[0m : ");
  printf(__func__);
  printf(" %dx%d\n", height, width);

}

void test_morpho_SIMD(void) {

  printf("TEST_MORPHO SIMD\n");
  printf("--------------------\n");
  test_erosion_losange_SIMD();
  test_dilatation_losange_SIMD();
  test_unitaire_SIMD0();
  test_unitaire_SIMD1();
  test_unitaire_SIMD2();
  test_unitaire_SIMD3();
  test_unitaire_SIMD4();
  test_unitaire_SIMD5();
  test_unitaire_SIMD6();
  test_unitaire_SIMD7();
  test_unitaire_SIMD8();
  test_unitaire_SIMD9();
  test_unitaire_SIMD10();
  test_unitaire_SIMD11();
  test_unitaire_SIMD12();
  test_unitaire_SIMD13();
  test_unitaire_SIMD14();
  test_unitaire_SIMD15();
  test_unitaire_SIMD16();
  test_unitaire_SIMD17();
  test_unitaire_SIMD18();
  test_unitaire_SIMD19();

  test_ouverture_universel(3, 3);
  test_ouverture_universel(10, 10);
  test_ouverture_universel(50, 50);
  test_ouverture_universel(128, 128);
  test_ouverture_universel(129, 129);

  test_fermeture_universel(3, 3);
  test_fermeture_universel(10, 10);
  test_fermeture_universel(50, 50);
  test_fermeture_universel(128, 128);
  test_fermeture_universel(129, 129);

  printf("====================\n");

  return;
}
