#ifndef BENCH_MOUVEMENT_H
  #define BENCH_MOUVEMENT_H

  #include <stdio.h>
  #include <stdlib.h>

  #include "nrutil.h"
  #include "nrdef.h"
  #include "utils.h"
  #include "vnrutil.h"
  #include "vnrdef.h"

  #include "mouvement.h"

  void mesure_routine_fd(void);
  void mesure_all_fd(void);

  void mesure_SigmaDelta_1step(void);

  void bench_mouvement(void);

#endif
