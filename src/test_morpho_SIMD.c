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

  free_vbitsmatrix(m, 3, 3);
  free_vbitsmatrix(erosion_result, 3, 3);
  free_vbitsmatrix(dilatation_result, 3, 3);
  free_vbitsmatrix(erosion_expected_result, 3, 3);
  free_vbitsmatrix(dilatation_expected_result, 3, 3);
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
  printf("====================\n");

  return;
}
