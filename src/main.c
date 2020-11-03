#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"

int main(void) {

	compute_all_fd();
	compute_all_sd_mean();
	compute_all_sd_diff();

	return 0;
}
