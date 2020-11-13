#include "morpho_SIMD.h"

// AND logique sur un voisinage de taille 3x3
vbits** erosion_3x3(vbits** img_bin, int height, int width){
  int nb_vbits_col = ceil((float)width/128);
  vbits** m = (vbits**)vui32matrix(0, height-1, 0, nb_vbits_col-1)

  vbits a0, b0, c0;
  vbits a1, b1, c1;
  vbits a2, b2, c2;

  vbits aa0, cc0;
  vbits aa1, cc1;
  vbits aa2, cc2;

  for(int i = 1; i < height-1; i++){
    for(int j = 1; j < nb_vbits_col-1; j++){

      a0 = vec_load(&img_bin[i-1][j-1]);
      b0 = vec_load(&img_bin[i-1][j-0]);
      c0 = vec_load(&img_bin[i-1][j+1]);

      a1 = vec_load(&img_bin[i-0][j-1]);
      b1 = vec_load(&img_bin[i-0][j-0]);
      c1 = vec_load(&img_bin[i-0][j+1]);

      a2 = vec_load(&img_bin[i+1][j-1]);
      b2 = vec_load(&img_bin[i+1][j-0]);
      c2 = vec_load(&img_bin[i+1][j+1]);

      

    }
  }
  return m;
}
