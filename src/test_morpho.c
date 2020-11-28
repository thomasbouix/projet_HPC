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

  uint8** m_ero = erosion_3x3(m_with_borders, height, width);

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

  uint8** m_res = dilatation_3x3(m_with_borders, height, width);
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

  printf("\033[1;32mSucess\033[0m : ");
  printf(__func__);
  printf(" %dx%d\n", height, width);

  free_padding_ui8matrix(m_with_borders, nrl-border, nrh+border, ncl-border, nch+border, 1);
  free_ui8matrix(m_res, nrl, nrh, ncl, nch);

  return;
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 0 0
*/

void test_unitaire0(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  uint8** m_with_borders = add_borders(m, 3, 3, 1);

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

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

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 0 0                 0 0 0                         1 1 0
  0 0 0                 0 0 0                         1 1 0
  0 0 0                 0 0 0                         0 0 0
*/

void test_unitaire1(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[0][0] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[0][0] = dilatation_expected_result[0][1] = dilatation_expected_result[1][0] = dilatation_expected_result[1][1] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 1 0                 0 0 0                         1 1 1
  0 0 0                 0 0 0                         1 1 1
  0 0 0                 0 0 0                         0 0 0
*/

void test_unitaire2(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[0][1] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[0][0] = dilatation_expected_result[0][1] = dilatation_expected_result[0][2] \
  = dilatation_expected_result[1][0] = dilatation_expected_result[1][1] = dilatation_expected_result[1][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 1                 0 0 0                         0 1 1
  0 0 0                 0 0 0                         0 1 1
  0 0 0                 0 0 0                         0 0 0
*/

void test_unitaire3(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[0][2] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[0][1] = dilatation_expected_result[0][2] \
  = dilatation_expected_result[1][1] = dilatation_expected_result[1][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         1 1 0
  1 0 0                 0 0 0                         1 1 0
  0 0 0                 0 0 0                         1 1 0
*/

void test_unitaire4(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[1][0] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[0][0] = dilatation_expected_result[0][1]  \
  = dilatation_expected_result[1][0] = dilatation_expected_result[1][1] \
  = dilatation_expected_result[2][0] = dilatation_expected_result[2][1] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}


/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         1 1 1
  0 1 0                 0 0 0                         1 1 1
  0 0 0                 0 0 0                         1 1 1
*/

void test_unitaire5(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[1][1] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[0][0] = dilatation_expected_result[0][1] = dilatation_expected_result[0][2] \
  = dilatation_expected_result[1][0] = dilatation_expected_result[1][1] = dilatation_expected_result[1][2] \
  = dilatation_expected_result[2][0] = dilatation_expected_result[2][1] = dilatation_expected_result[2][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 1 1
  0 0 1                 0 0 0                         0 1 1
  0 0 0                 0 0 0                         0 1 1
*/

void test_unitaire6(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[1][2] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[0][1] = dilatation_expected_result[0][2] \
  = dilatation_expected_result[1][1] = dilatation_expected_result[1][2] \
  = dilatation_expected_result[2][1] = dilatation_expected_result[2][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         1 1 0
  1 0 0                 0 0 0                         1 1 0
*/

void test_unitaire7(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[2][0] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[1][0] = dilatation_expected_result[1][1] \
  = dilatation_expected_result[2][0] = dilatation_expected_result[2][1] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         1 1 1
  0 1 0                 0 0 0                         1 1 1
*/

void test_unitaire8(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[2][1] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[1][0] = dilatation_expected_result[1][1] = dilatation_expected_result[1][2] \
  = dilatation_expected_result[2][0] = dilatation_expected_result[2][1] = dilatation_expected_result[2][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 0 0                 0 0 0                         0 0 0
  0 0 0                 0 0 0                         0 1 1
  0 0 1                 0 0 0                         0 1 1
*/

void test_unitaire9(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  zero_ui8matrix(&m, 0, 2, 0, 2);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  zero_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2);

  m[2][2] = 1;
  m_with_borders = add_borders(m, 3, 3, 1);

  dilatation_expected_result[1][1] = dilatation_expected_result[1][2] \
  = dilatation_expected_result[2][1] = dilatation_expected_result[2][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/

void test_unitaire10(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  m_with_borders = add_borders(m, 3, 3, 1);


  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != 1){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != 1){
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

/*Pattern       expected_result_erosion     expected_result_dilatation
  0 1 1                 0 0 1                         1 1 1
  1 1 1                 0 0 1                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/

void test_unitaire11(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  set_ui8matrix(&erosion_expected_result, 0, 2, 0, 2, 1);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[0][0] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[0][0] = erosion_expected_result[0][1] \
  = erosion_expected_result[1][0] = erosion_expected_result[1][1] = 0;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);

  // display_ui8matrix(erosion_result, 0, 2, 0, 2, "%d", "erosion_result");

  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("[%d][%d]\n", i, j);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 0 1                 0 0 0                         1 1 1
  1 1 1                 0 0 0                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/

void test_unitaire12(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[0][1] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[2][0] = erosion_expected_result[2][1] = erosion_expected_result[2][2] = 1 ;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 0                 1 0 0                         1 1 1
  1 1 1                 1 0 0                         1 1 1
  1 1 1                 1 1 1                         1 1 1
*/

void test_unitaire13(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  set_ui8matrix(&erosion_expected_result, 0, 2, 0, 2, 1);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[0][2] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[0][1] = erosion_expected_result[0][2] \
  = erosion_expected_result[1][1] = erosion_expected_result[1][2] =0 ;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 0 0 1                         1 1 1
  0 1 1                 0 0 1                         1 1 1
  1 1 1                 0 0 1                         1 1 1
*/

void test_unitaire14(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[1][0] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[0][2] = erosion_expected_result[1][2] = erosion_expected_result[2][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 0 0 0                         1 1 1
  1 0 1                 0 0 0                         1 1 1
  1 1 1                 0 0 0                         1 1 1
*/

void test_unitaire15(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[1][1] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 0 0                         1 1 1
  1 1 0                 1 0 0                         1 1 1
  1 1 1                 1 0 0                         1 1 1
*/

void test_unitaire16(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[1][2] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[0][0] = erosion_expected_result[1][0] = erosion_expected_result[2][0] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 0 0 1                         1 1 1
  0 1 1                 0 0 1                         1 1 1
*/

void test_unitaire17(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  set_ui8matrix(&erosion_expected_result, 0, 2, 0, 2, 1);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[2][0] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[1][0] = erosion_expected_result[1][1] \
  = erosion_expected_result[2][0] = erosion_expected_result[2][1] = 0;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 0 0 0                         1 1 1
  1 0 1                 0 0 0                         1 1 1
*/

void test_unitaire18(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  zero_ui8matrix(&erosion_expected_result, 0, 2, 0, 2);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[2][1] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[0][0] = erosion_expected_result[0][1] = erosion_expected_result[0][2] = 1;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

/*Pattern       expected_result_erosion     expected_result_dilatation
  1 1 1                 1 1 1                         1 1 1
  1 1 1                 1 0 0                         1 1 1
  1 1 0                 1 0 0                         1 1 1
*/

void test_unitaire19(void){
  uint8 **erosion_result, **dilatation_result;
  uint8 **erosion_expected_result, **dilatation_expected_result;
  uint8 ** m = ui8matrix(0, 2, 0, 2);
  uint8** m_with_borders;
  erosion_expected_result = ui8matrix(0, 2, 0, 2);
  dilatation_expected_result = ui8matrix(0, 2, 0, 2);
  set_ui8matrix(&m, 0, 2, 0, 2, 1);
  set_ui8matrix(&erosion_expected_result, 0, 2, 0, 2, 1);
  set_ui8matrix(&dilatation_expected_result, 0, 2, 0, 2, 1);

  m[2][2] = 0;
  m_with_borders = add_borders(m, 3, 3, 1);

  erosion_expected_result[1][1] = erosion_expected_result[1][2] \
  = erosion_expected_result[2][1] = erosion_expected_result[2][2] = 0;

  erosion_result = erosion_3x3(m_with_borders, 3, 3);
  dilatation_result = dilatation_3x3(m_with_borders, 3, 3);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(dilatation_result[i][j] != dilatation_expected_result[i][j]){
        printf("%d\n", dilatation_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(erosion_result[i][j] != erosion_expected_result[i][j]){
        printf("%d\n", erosion_result[i][j]);
        ERROR(__func__);
      }
    }
  }

  SUCCESS(__func__);

  free_ui8matrix(m, 0, 2, 0, 2);
  free_ui8matrix(erosion_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_result, 0, 2, 0, 2);
  free_ui8matrix(erosion_expected_result, 0, 2, 0, 2);
  free_ui8matrix(dilatation_expected_result, 0, 2, 0, 2);
  free_padding_ui8matrix(m_with_borders, -1, 3, -1, 3, 1);
}

void test_morpho(void) {

  printf("TEST MORPHO SCALAIRE\n");
  printf("--------------------\n");
  test_ero_5x7_losange_plein();
  test_dil_bordure(3, 3);
  test_dil_bordure(5, 5);
  test_dil_bordure(10, 10);
  test_dil_bordure(100, 100);
  test_unitaire0();
  test_unitaire1();
  test_unitaire2();
  test_unitaire3();
  test_unitaire3();
  test_unitaire4();
  test_unitaire5();
  test_unitaire6();
  test_unitaire7();
  test_unitaire8();
  test_unitaire9();
  test_unitaire10();
  test_unitaire11();
  test_unitaire12();
  test_unitaire13();
  test_unitaire14();
  test_unitaire15();
  test_unitaire16();
  test_unitaire17();
  test_unitaire18();
  test_unitaire19();
  printf("====================\n");

  return;
}
