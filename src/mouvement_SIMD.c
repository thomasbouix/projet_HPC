#include "mouvement_SIMD.h"

#define N 4
#define V_MIN 1
#define V_MAX 254

vuint8** routine_FrameDifference_SIMD(vuint8** I_t, vuint8** I_t_moins_1, int vi0, int vi1, int vj0, int vj1, int theta)
{
  vuint8 ** E_t = vui8matrix(vi0, vi1, vj0, vj1);
  vuint8 vec_img0, vec_img1, abs_diff;

  for(int i = vi0; i <= vi1; i++){
    for(int j = vj0; j <= vj1; j++){
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
  int si0, si1, sj0, sj1;
  int vi0, vi1, vj0, vj1;
  char buff[35];
  uint8** output_img = NULL;
  vuint8** img1 = NULL;
  vuint8** img0 = LoadPGM_vui8matrix("car3/car_3000.pgm", &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);

  if(save)
    CHECK_ERROR(system("mkdir -p output/fd_SIMD"));

  for(int i = 1; i < 200; i++){
    sprintf(buff, "car3/car_3%.3d.pgm", i);
    img1 = LoadPGM_vui8matrix(buff, &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);
    output_img = (uint8**) routine_FrameDifference_SIMD(img1, img0, vi0, vi1, vj0, vj1, threshold);
    if(save){
      sprintf(buff, "output/fd_SIMD/out_3%.3d.pgm", i);
      SavePGM_ui8matrix(output_img, si0, si1, sj0, sj1, buff);
    }
    free_ui8matrix(output_img, si0, si1, sj0, sj1);
    free_vui8matrix(img0, vi0, vi1, vj0, vj1);
    img0 = img1;
  }
}


void SigmaDelta_step0_SIMD(uint8*** M_0, uint8*** V_0, int* nrl, int* nrh, int* ncl, int* nch)
{
  *M_0 = LoadPGM_ui8matrix("car3/car_3000.pgm", nrl, nrh, ncl, nch);
  *V_0 = ui8matrix(*nrl,*nrh, *ncl, *nch);
  vuint initial_V = init_vuint8(V_MIN);
  for(int i = *nrl; i <= *nrh; i++){
    for(int j = *ncl; j <= *nch; j+=16){
      (*V_0)[i][j] = V_MIN;
      vec_store(&(*V_0)[i][j], initial_V);
    }
  }
}
