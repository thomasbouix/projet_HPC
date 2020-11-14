#ifndef BENCH_MORPHO_SIMD_H
  #define BENCH_MORPHO_SIMD_H

  #include <stdio.h>
  #include <stdlib.h>

  #include "nrutil.h"
  #include "nrdef.h"
  #include "utils.h"
  #include "vnrutil.h"
  #include "vnrdef.h"

  #include "morpho_SIMD.h"

  void bench_erosion_SIMD(void);
  void bench_dilatation_SIMD(void);
  void bench_morpho_SIMD(void);

#endif
