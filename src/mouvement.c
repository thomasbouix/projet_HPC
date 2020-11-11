#include "mouvement.h"

#define N 4
#define V_MIN 1
#define V_MAX 254

uint8** routine_FrameDifference(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta){

  uint8** E_t = ui8matrix(nrl, nrh, ncl, nch);
  // uint8** O_t = ui8matrix(nrl, nrh, ncl, nch);

  for(int i = nrl; i <= nrh; i++){
    for(int j = ncl; j <= nch; j++){
      if(abs(I_t[i][j] - I_t_moins_1[i][j]) < theta)
        E_t[i][j] = (uint8)0;
      else
        E_t[i][j] = (uint8)255;
      // O_t[i][j] = (uint8)abs(I_t[i][j] - I_t_moins_1[i][j]);
    }
  }
  return E_t;
}

void compute_fd(int num_image, int threshold, int save){
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
  if(save){
    sprintf(buff, "output/fd/E_t_%.3d.pgm", num_image);
    SavePGM_ui8matrix(outpout_img, nrl1, nrh1, ncl1, nch1, buff);
  }

  free_ui8matrix(outpout_img, nrl1, nrh1, ncl1, nch1);
  free_ui8matrix(img1, nrl1, nrh1, ncl1, nch1);
  free_ui8matrix(img2, nrl1, nrh1, ncl1, nch1);
}

void compute_fd_all_steps(int threshold, int save){
  if(save)
    CHECK_ERROR(system("mkdir -p output/fd"));

  for(int i = 1; i < 200; i++)
    compute_fd(i, threshold, save);
}


void SigmaDelta_step0(uint8*** M_0, uint8*** V_0, int* nrl, int* nrh, int* ncl, int* nch){
  *M_0 = LoadPGM_ui8matrix("car3/car_3000.pgm", nrl, nrh, ncl, nch);
  *V_0 = ui8matrix(*nrl,*nrh, *ncl, *nch);
  for(int i = *nrl; i <= *nrh; i++){
    for(int j = *ncl; j <= *nch; j++){
      (*V_0)[i][j] = V_MIN;
    }
  }
}

uint8** SigmaDelta_1step(uint8** M_t_moins_1, uint8*** M_t_save, uint8** V_t_moins_1, uint8*** V_t_save, uint8** I_t, int nrl, int nrh, int ncl, int nch){
  uint8** M_t = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V_t = ui8matrix(nrl, nrh, ncl, nch);
  uint8 O_t, N_O_t;
  uint8** E_t = ui8matrix(nrl, nrh, ncl, nch);
  //printf("\nnrl:%d, nrh:%d, ncl:%d, nch:%d\n", nrl, nrh, ncl, nch);
  for(int i = nrl; i <= nrh; i++){
    for(int j = ncl; j <= nch; j++){
      //printf("i:%d,j:%d\t",i,j);

      if(M_t_moins_1[i][j] < I_t[i][j])
        M_t[i][j] = M_t_moins_1[i][j] + 1;
      else if(M_t_moins_1[i][j] > I_t[i][j])
        M_t[i][j] = M_t_moins_1[i][j] - 1;
      else
        M_t[i][j] = M_t_moins_1[i][j];

      O_t = abs(M_t[i][j] - I_t[i][j]);
      N_O_t = MIN(N*O_t, 255);

      if(V_t_moins_1[i][j] < N_O_t)
        V_t[i][j] = V_t_moins_1[i][j] + 1;
      else if(V_t_moins_1[i][j] > N_O_t)
        V_t[i][j] = V_t_moins_1[i][j] - 1;
      else
        V_t[i][j] = V_t_moins_1[i][j];

      V_t[i][j] = MAX(MIN(V_t[i][j], V_MAX), V_MIN);

      if(O_t < V_t[i][j])
        E_t[i][j] = (uint8)0;
      else
        E_t[i][j] = (uint8)255;
    }
  }

  *M_t_save = M_t;
  *V_t_save = V_t;
  return E_t;
}

void compute_sd_all_steps(void){
  CHECK_ERROR(system("mkdir -p output/sd"));

  uint8** M_t_moins_1 = NULL;
  uint8** V_t_moins_1 = NULL;
  uint8** E_t = NULL;
  uint8** I_t = NULL;
  uint8** M_t = NULL;
  uint8** V_t = NULL;
  int nrl, nrh, ncl, nch;
  char buff[30];
  SigmaDelta_step0(&M_t_moins_1, &V_t_moins_1, &nrl, &nrh, &ncl, &nch);

  for(int i = 1; i < 200; i++){
    //printf("\n=========== %d ===========\n", i);
    sprintf(buff, "car3/car_3%.3d.pgm", i);
    I_t = LoadPGM_ui8matrix(buff, &nrl, &nrh, &ncl, &nch);
    E_t = SigmaDelta_1step(M_t_moins_1, &M_t, V_t_moins_1, &V_t, I_t, nrl, nrh, ncl, nch);
    sprintf(buff, "output/sd/E_t_%.3d.pgm", i);
    SavePGM_ui8matrix(E_t, nrl, nrh, ncl, nch, buff);
    free_ui8matrix(M_t_moins_1, nrl, nrh, ncl, nch);
    free_ui8matrix(V_t_moins_1, nrl, nrh, ncl, nch);
    M_t_moins_1 = M_t;
    V_t_moins_1 = V_t;
    free_ui8matrix(I_t, nrl, nrh, ncl, nch);
    free_ui8matrix(E_t, nrl, nrh, ncl, nch);
  }
  free_ui8matrix(M_t, nrl, nrh, ncl, nch);
  free_ui8matrix(V_t, nrl, nrh, ncl, nch);
}
