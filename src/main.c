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
#include "morpho_SIMD.h"

#include "test_morpho.h"
#include "test_morpho_SIMD.h"

#include "bench_mouvement.h"
#include "bench_mouvement_SIMD.h"
#include "bench_morpho.h"
#include "bench_morpho_SIMD.h"


int main(int argc, char * argv[]) {

  // test_morpho();
  // test_morpho_SIMD();
  //
  // bench_mouvement();
  // bench_mouvement_SIMD();
  //
  // bench_morpho();
  bench_morpho_SIMD();

  return 0;
}
