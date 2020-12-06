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

#include "bench_chaine_complete.h"



int main(int argc, char * argv[]) {

  test_morpho();
  test_morpho_SIMD();

  bench_mouvement();
  bench_mouvement_SIMD();

  bench_morpho();
  bench_morpho_SIMD();

  /*
   *  Pour les fonctions bench_CSV, penser à décommenter la macro BENCH
   *  dans le fichier utils.h (ligne 4)
   *  (pour éviter les fuites mémoires)
   */

  /*
      bench_chaine_complete_csv(3, 2000, 1);
      bench_mouvement_csv(16, 2400, 16);
  */


  return 0;
}
