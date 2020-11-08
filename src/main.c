#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "nrutil.h"
#include "nrdef.h"
#include "utils.h"
#include "vnrutil.h"
#include "vnrdef.h"

#include "mouvement.h"
#include "mouvement_SIMD.h"
#include "morpho.h"

#include "bench_mouvement.h"
#include "bench_mouvement_SIMD.h"

int main(void){

  bench_mouvement_SIMD();
  bench_mouvement();

  return 0;
}
