#ifndef MORPHO_H
#define MORPHO_H

  #include <stdio.h>
  #include <stdlib.h>
  #include "nrutil.h"
  #include "utils.h"

  #define IMG_ROWS 239
  #define IMG_COLS 319

  uint8** erosion_3x3         (uint8** img_with_padding, int height, int width);
  uint8** dilatation_3x3      (uint8** img_with_padding, int height, int width);
  uint8** ouverture_3x3       (uint8** img_with_padding, int height, int width);
  uint8** fermeture_3x3       (uint8** img_with_padding, int height, int width);
  uint8** chaine_complete_3x3 (uint8** img_with_padding, int height, int width);

  void compute_erosion3x3     (char* basePath, int save);
  void compute_dilatation_3x3 (char* basePath, int save);

#endif
