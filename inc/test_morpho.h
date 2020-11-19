#ifndef TEST_MORHO_H
  #define TEST_MORHO_H

  #include <stdio.h>
  #include <stdlib.h>

  #include "nrdef.h"
  #include "nrutil.h"
  #include "simd_macro.h"
  #include "utils.h"
  #include "vnrdef.h"
  #include "vnrutil.h"
  #include "morpho.h"
  #include "mouvement.h"

  void test_ero_5x7_losange_plein(void);
  // test une dilatation sur une image de taille height x width et des bordures à 1
  void test_dil_bordure(int height, int width);
  void test_morpho(void);

#endif
