#ifndef MORPHO_H
#define MORPHO_H

  #include <stdio.h>
  #include <stdlib.h>
  #include "nrutil.h"
  #include "utils.h"


  #define IMG_ROWS 239
  #define IMG_COLS 319

  uint8** erosion(uint8** img_with_padding, int height, int width, int kernel_size);
  uint8** dilatation(uint8** img_with_padding, int height, int width, int kernel_size);
  uint8** ouverture(uint8** img_with_padding, int height, int width, int kernel_size);
  void compute_erosion(char* basePath, int kernel_size, int save);
  void compute_dilatation(char* basePath, int kernel_size, int save);

#endif
