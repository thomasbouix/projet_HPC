#ifndef TEST_MORHO__SIMD_H
  #define TEST_MORHO__SIMD_H

  #include <stdio.h>
  #include <stdlib.h>

  #include "nrdef.h"
  #include "nrutil.h"
  #include "simd_macro.h"
  #include "utils.h"
  #include "vnrdef.h"
  #include "vnrutil.h"
  #include "morpho_SIMD.h"
  #include "mouvement.h"

  void test_erosion_losange_SIMD(void);
  void test_dilatation_losange_SIMD(void);
  void test_unitaire_SIMD0(void);
  void test_morpho_SIMD(void);

#endif
