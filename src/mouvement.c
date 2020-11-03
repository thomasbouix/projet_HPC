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
void compute_all_sd_mean(void) {

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
		uint8** mean_i_ = routine_Mean(mean_i_moins_1, car3_i_, nrl, nrh, ncl, nch);

		SavePGM_ui8matrix(mean_i_, nrl, nrh, ncl, nch, mean_path);
		free_ui8matrix(car3_i_, nrl, nrh, ncl, nch);
		free_ui8matrix(mean_i_moins_1, nrl, nrh, ncl, nch);
		free_ui8matrix(mean_i_, nrl, nrh, ncl, nch);
	}

	return;
}
void compute_all_sd_diff() {

	CHECK_ERROR(system("mkdir -p output/sd/diff"));

	int nrl, nrh, ncl, nch;
	char car_i_[30];					//
	char moyenne_i_[30];				// output/sd/mean/mean000.pgm
	char diff[30];						// output/sd/diff/diff000.pgm

	for (int i=1; i<=199; i++) {

		sprintf(car_i_, "car3/car_3%.3d.pgm", i);
		sprintf(moyenne_i_, "output/sd/mean/mean%.3d.pgm", i);
		sprintf(diff, "output/sd/diff/diff%.3d.pgm", i);
		uint8** M_t = LoadPGM_ui8matrix(moyenne_i_, &nrl, &nrh, &ncl, &nch);
		uint8** I_t = LoadPGM_ui8matrix(car_i_, &nrl, &nrh, &ncl, &nch);
		uint8** O_t = ui8matrix(nrl, nrh, ncl, nch);

		for(int i = nrl; i <= nrh; i++){
			for(int j = ncl; j <= nch; j++){
					O_t[i][j] = abs(M_t[i][j] - I_t[i][j]);
			}
		}
		SavePGM_ui8matrix(O_t, nrl, nrh, ncl, nch, diff);
		free_ui8matrix(M_t, nrl, nrh, ncl, nch);
		free_ui8matrix(I_t, nrl, nrh, ncl, nch);
		free_ui8matrix(O_t, nrl, nrh, ncl, nch);
	}

	return;
}
void compute_all_sd_up_cl() {

	CHECK_ERROR(system("mkdir -p output/sd/up_cl"));

	uint8 Vmin = 1;
	uint8 Vmax = 254;
	int N = 4;

	int nrl = 0;
	int nrh = 239;
	int ncl = 0;
	int nch = 3190;

	char diff_path[30];					// output/sd/diff/diff001.pgm
	char v_i_moins_1_path[60];			// output/sd/variance/v000.pgm
	char v_path[60];					// output/sd/variance/v001.pgm

	/* ------------ Initialisation ------------ */
	uint8** v3000 = ui8matrix(nrl, nrh, ncl, nch);
	for (int i=nrl; i<=nrh; i++) {
		for (int j=ncl; j<=nch; j++) {
			v3000[i][j] = Vmin;
		}
	}
	SavePGM_ui8matrix(v3000, nrl, nrh, ncl, nch, "output/sd/up_cl/v000.pgm");
	free_ui8matrix(v3000, nrl, nrh, ncl, nch);

	/* ---------------------------------------- */
	for (int i=1; i<=199; i++) {

		sprintf(diff_path, "output/sd/diff/diff%.3d.pgm", i);
		sprintf(v_i_moins_1_path, "output/sd/up_cl/v%.3d.pgm", i-1);
		sprintf(v_path, "output/sd/up_cl/v%.3d.pgm", i);
		uint8** V_t_moins_1 = LoadPGM_ui8matrix(v_i_moins_1_path, &nrl, &nrh, &ncl, &nch);
		uint8** O_t = LoadPGM_ui8matrix(diff_path, &nrl, &nrh, &ncl, &nch);

		uint8** V_t = ui8matrix(nrl, nrh, ncl, nch);

		for(int i = nrl; i <= nrh; i++){
			for(int j = ncl; j <= nch; j++){
				if (V_t_moins_1[i][j] < N * O_t[i][j]) {
					V_t[i][j] = V_t_moins_1[i][j] + 1;
				} else if (V_t_moins_1[i][j] > N * O_t[i][j]) {
					V_t[i][j] = V_t_moins_1[i][j] - 1;
				} else {
					V_t[i][j] = V_t_moins_1[i][j];
				}
				V_t[i][j] = MAX(MIN(V_t[i][j], Vmax), Vmin);
			}
		}
		SavePGM_ui8matrix(V_t, nrl, nrh, ncl, nch, v_path);
		free_ui8matrix(V_t_moins_1, nrl, nrh, ncl, nch);
		free_ui8matrix(O_t, nrl, nrh, ncl, nch);
		free_ui8matrix(V_t, nrl, nrh, ncl, nch);
	}

	return;
}
void compute_all_sd_est() {

	CHECK_ERROR(system("mkdir -p output/sd/estimation"));

	int nrl, nrh, ncl, nch;

	char diff_i_[40];					// output/sd/diff/diff000.pgm
	char variance_i_[40];				// output/sd/variance/v001.pgm
	char estimation_i_[50];				// output/sd/estimation/e001.pgm

	for (int i=1; i<=199; i++) {
		sprintf(diff_i_, "output/sd/diff/diff%.3d.pgm", i);
		sprintf(variance_i_, "output/sd/up_cl/v%.3d.pgm", i);
		sprintf(estimation_i_, "output/sd/estimation/e%.3d.pgm", i);
		uint8** O_t = LoadPGM_ui8matrix(diff_i_, &nrl, &nrh, &ncl, &nch);
		uint8** V_t = LoadPGM_ui8matrix(variance_i_, &nrl, &nrh, &ncl, &nch);
		uint8** E_t = ui8matrix(nrl, nrh, ncl, nch);

		for(int i = nrl; i <= nrh; i++) {
			for(int j = ncl; j <= nch; j++) {
				if (O_t[i][j] < V_t[i][j]) {
					E_t[i][j] = 0;
				} else {
					E_t[i][j] = 255;
				}
			}
		}
		SavePGM_ui8matrix(E_t, nrl, nrh, ncl, nch, estimation_i_);
		free_ui8matrix(O_t, nrl, nrh, ncl, nch);
		free_ui8matrix(V_t, nrl, nrh, ncl, nch);
		free_ui8matrix(E_t, nrl, nrh, ncl, nch);
	}
	return;
}
