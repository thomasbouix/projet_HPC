#include "mouvement_SIMD.h"


uint8** routine_FrameDifference_SIMD(uint8** I_t, uint8** I_t_moins_1, int nrl, int nrh, int ncl, int nch, int theta)
{
  uint8 ** E_t = ui8matrix(nrl, nrh, ncl, nch);
  vuint8 vec_img0, vec_img1, abs_diff;

  for(int i = nrl; i <= nrh; i++){
    for(int j = ncl; j <= nch; j+=16){
      vec_img0 = vec_load(&I_t_moins_1[i][j]);
      vec_img1 = vec_load(&I_t[i][j]);

      abs_diff = vABS_DIFF(vec_img0, vec_img1);
      vec_store(&E_t[i][j], vCMP_THRESHOLD(abs_diff, theta));
    }
  }
  return E_t;
}

void compute_fd_SIMD(int threshold, int save)
{
  int nrl, nrh, ncl, nch;
  char buff[35];
  uint8** output_img = NULL;
  uint8** img1 = NULL;
  uint8** img0 = LoadPGM_ui8matrix("car3/car_3000.pgm", &nrl, &nrh, &ncl, &nch);

  if(save)
    CHECK_ERROR(system("mkdir -p output/fd_SIMD"));

  for(int i = 1; i < 200; i++){
    sprintf(buff, "car3/car_3%.3d.pgm", i);
    img1 = LoadPGM_ui8matrix(buff, &nrl, &nrh, &ncl, &nch);
    output_img = routine_FrameDifference_SIMD(img1, img0, nrl, nrh, ncl, nch, threshold);
    if(save){
      sprintf(buff, "output/fd_SIMD/out_3%.3d.pgm", i);
      SavePGM_ui8matrix(output_img, nrl, nrh, ncl, nch, buff);
    }
    free_ui8matrix(output_img, nrl, nrh, ncl, nch);
    free_ui8matrix(img0, nrl, nrh, ncl, nch);
    img0 = img1;
  }
}
