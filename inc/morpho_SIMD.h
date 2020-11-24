#ifndef MORPHO_SIMD_H
#define MORPHO_SIMD_H

  #include <stdio.h>
  #include "nrutil.h"
  #include "utils.h"
  #include "vnrutil.h"

  vbits** erosion_3x3_SIMD(vbits** img_bin, int height, int width);
  void compute_all_erosion_3x3_SIMD(char* basePath, int save);

  vbits** dilatation_3x3_SIMD(vbits** img_bin, int height, int width);
  void compute_all_dilatation_3x3_SIMD(char* basePath, int save);

  vbits** ouverture_SIMD(vbits** img_bin, int height, int width);
  vbits ** ouverture_naive_SIMD(vbits** img_bin, int height, int width);

  vbits** fermeture_SIMD(vbits** img_bin, int height, int width);
  vbits ** fermeture_naive_SIMD(vbits** img_bin, int height, int width);

  vbits ** chaine_complete_naive_SIMD(vbits** img_bin, int height, int width);
  vbits ** chaine_complete_fusion_SIMD(vbits** img_bin, int height, int width);

#endif
