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

  void test_unitaire0(void);
  void test_unitaire1(void);
  void test_unitaire2(void);
  void test_unitaire3(void);
  void test_unitaire4(void);
  void test_unitaire5(void);
  void test_unitaire6(void);
  void test_unitaire7(void);
  void test_unitaire8(void);
  void test_unitaire9(void);
  void test_unitaire10(void);
  void test_unitaire11(void);
  void test_unitaire12(void);
  void test_unitaire13(void);
  void test_unitaire14(void);
  void test_unitaire15(void);
  void test_unitaire16(void);
  void test_unitaire17(void);
  void test_unitaire18(void);
  void test_unitaire19(void);

  void test_morpho(void);

#endif
