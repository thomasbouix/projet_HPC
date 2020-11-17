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

  void ero_losange_plein(void);
  void test_dilatation(void);
  void test_morpho(void);

#endif
