#include "mouvement.h"

uint8** routine_FrameDifference(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta){

	uint8** E_t = ui8matrix(nrl, nrh, ncl, nch);
	// uint8** O_t = ui8matrix(nrl, nrh, ncl, nch);

	for(int i = nrl; i <= nrh; i++){
		for(int j = ncl; j <= nch; j++){
			if(abs(I_t[i][j] - I_t_moins_1[i][j]) < theta)
				E_t[i][j] = (uint8)0;
			else
				E_t[i][j] = (uint8)255;
		}
	}
	return E_t;
}

/* ---- FD ----- */
void compute_fd(int num_image, int threshold){
	int nrl1, nrh1, ncl1, nch1, nrl2, nrh2, ncl2, nch2;
	char buff[30];

	sprintf(buff, "car3/car_3%.3d.pgm", (num_image - 1));
	uint8** img1 = LoadPGM_ui8matrix(buff, &nrl1, &nrh1, &ncl1, &nch1);

	sprintf(buff, "car3/car_3%.3d.pgm", num_image);
	uint8** img2 = LoadPGM_ui8matrix(buff, &nrl2, &nrh2, &ncl2, &nch2);

	if( ((nrh1 - nrl1) != (nrh2 - nrl2)) || (nch1 - ncl1) != (nch2 - ncl2)){
		fprintf(stderr, "***Error : Images must have the same size !\n");
	}

	uint8** outpout_img = routine_FrameDifference(img2, img1, nrl1, nrh1, ncl1, nch1, threshold);
	sprintf(buff, "output/fd/diff%.3d.pgm", num_image);
	SavePGM_ui8matrix(outpout_img, nrl1, nrh1, ncl1, nch1, buff);

	free(outpout_img);
}
void compute_all_fd(void) {

	CHECK_ERROR(system("mkdir -p output/fd"));

	for(int i = 1; i < 200; i++)
		compute_fd(i, 50);

	return;
}

/* ---- SD ----- */
uint8** routine_Mean(uint8** M_t_moins_1, uint8** I_t, int nrl, int nrh, int ncl, int nch) {

	uint8** M_t = ui8matrix(nrl, nrh, ncl, nch);

	for (int i=nrl; i<=nrh; i++) {
		for (int j=ncl; j<=nch; j++) {
			if ( M_t_moins_1[i][j] < I_t[i][j] ) {
				M_t[i][j] = M_t_moins_1[i][j] + 1;
			} else if ( M_t_moins_1[i][j] > I_t[i][j] ) {
				M_t[i][j] = M_t_moins_1[i][j] - 1;
			} else {
				M_t[i][j] = M_t_moins_1[i][j];
			}
		}
	}
	return M_t;
}

void compute_all_means(void) {

	CHECK_ERROR(system("mkdir -p output/sd/mean"));

	int nrl, nrh, ncl, nch;
	char car3_path[30];					// car3/car3001.pgm
	char mean_i_moins_1_path[60];		// output/sd/mean/mean000.pgm
	char mean_path[60];					// output/sd/mean/mean001.pgm

	/* ------------ Initialisation ------------ */
	uint8** car3000 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);
	SavePGM_ui8matrix(car3000, nrl, nrh, ncl, nch, "output/sd/mean/mean000.pgm");
	free_ui8matrix(car3000, nrl, nrh, ncl, nch);
	/* ---------------------------------------- */

	for (int i=1; i<=199; i++) {
		sprintf(car3_path, "car3/car_3%.3d.pgm", i);
		sprintf(mean_i_moins_1_path, "output/sd/mean/mean%.3d.pgm", i-1);
		sprintf(mean_path, "output/sd/mean/mean%.3d.pgm", i);

		uint8** car3_i_ = LoadPGM_ui8matrix(car3_path, &nrl, &nrh, &ncl, &nch);
		uint8** mean_i_moins_1 = LoadPGM_ui8matrix(mean_i_moins_1_path, &nrl, &nrh, &ncl, &nch);
		uint8** mean_i_ = routine_Mean(mean_i_moins_1,car3_i_, nrl, nrh, ncl, nch);

		SavePGM_ui8matrix(mean_i_, nrl, nrh, ncl, nch, mean_path);
		free_ui8matrix(car3_i_, nrl, nrh, ncl, nch);
		free_ui8matrix(mean_i_moins_1, nrl, nrh, ncl, nch);
		free_ui8matrix(mean_i_, nrl, nrh, ncl, nch);
	}

	return;
}
