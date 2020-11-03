#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"

int main(void) {

	// compute_all_fd();
	compute_all_sd_mean();
	compute_all_sd_diff();
	compute_all_sd_up_cl();

	return 0;
}
