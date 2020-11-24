#ifndef BENCH_MORPHO_H
  #define BENCH_MORPHO_H

  #include <stdio.h>
  #include <stdlib.h>

  #include "nrutil.h"
  #include "nrdef.h"
  #include "utils.h"
  #include "vnrutil.h"
  #include "vnrdef.h"

  #include "morpho.h"

  void bench_erosion(void);
  void bench_dilatation(void);
  void bench_ouverture(void);
  void bench_fermeture(void);
  void bench_chaine_complete(void);
  void bench_morpho(void);

#endif
