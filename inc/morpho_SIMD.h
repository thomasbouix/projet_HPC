#ifndef MORPHO_SIMD_H
#define MORPHO_SIMD_H

  #include <stdio.h>
  #include "nrutil.h"
  #include "utils.h"
  #include "vnrutil.h"

  vbits** erosion_3x3_SIMD_naif(vbits** img_bin, int height, int width);
  vbits** dilatation_3x3_SIMD_naif(vbits** img_bin, int height, int width);
  vbits** ouverture_SIMD_naif(vbits** img_bin, int height, int width);
  vbits** fermeture_SIMD_naif(vbits** img_bin, int height, int width);
  vbits** chaine_complete_SIMD_naif(vbits** img_bin, int height, int width);

  vbits** erosion_3x3_SIMD_opti(vbits** img_bin, int height, int width);
  vbits** dilatation_3x3_SIMD_opti(vbits** img_bin, int height, int width);
  vbits** ouverture_opti_SIMD(vbits** img_bin, int height, int width);
  vbits** fermeture_opti_SIMD(vbits** img_bin, int height, int width);
  vbits** chaine_complete_opti_SIMD(vbits** img_bin, int height, int width);

  void compute_all_erosion_3x3_SIMD_opti(char* basePath, int save);
  void compute_all_dilatation_3x3_SIMD_opti(char* basePath, int save);

  vbits** ouverture_fusion_SIMD(vbits** img_bin, int height, int width);
  vbits** fermeture_fusion_SIMD(vbits** img_bin, int height, int width);
  vbits** chaine_complete_fusion_SIMD(vbits** img_bin, int height, int width);

  // m[i][j] bitwise
  int get_bit(vbits** m, int i, int j);

#endif
