#ifndef MORPHO_SIMD_H
#define MORPHO_SIMD_H

  #include <stdio.h>
  #include "nrutil.h"
  #include "utils.h"
  #include "vnrutil.h"

  vbits** erosion_3x3(vbits** img_bin, int height, int width);
  void compute_erosion_3x3_SIMD(char* basePath, int save);

#endif
