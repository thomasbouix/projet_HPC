#ifndef BENCH_MOUVEMENT_H
  #define BENCH_MOUVEMENT_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>

  #include "nrutil.h"
  #include "nrdef.h"
  #include "utils.h"
  #include "vnrutil.h"
  #include "vnrdef.h"

  #include "mouvement.h"

  void mesure_routine(void);
  void mesure_compute_fd(void);
  void bench_mouvement(void);

#endif
