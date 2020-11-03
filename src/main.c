#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "mouvement.h"

int main(void) {

	// compute_all_fd();

	int nrl, nrh, ncl, nch;

	uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
	uint8** img1 = LoadPGM_ui8matrix("car3/car_3100.pgm", &nrl, &nrh, &ncl, &nch);

	uint8** M1 = routine_Mean(img1, img0, nrl, nrh, ncl, nch);

	SavePGM_ui8matrix(M1, nrl, nrh, ncl, nch, "moyenne");

	return 0;
}
