#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"


int main(void){

  system("mkdir -p output/fd");

  for(int i = 1; i < 200; i++)
    compute_fd(i, 50);

}
