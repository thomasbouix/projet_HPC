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
    img1 = LoadPGM_vui8matrix("car3/car_3000.pgm", &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);
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


void SigmaDelta_step0_SIMD(vuint8*** M_0, vuint8*** V_0, int* vi0, int* vi1, int* vj0, int* vj1)
{
  int si0, si1, sj0, sj1;
  *M_0 = LoadPGM_vui8matrix("car3/car_3000.pgm", &si0, &si1, &sj0, &sj1, vi0, vi1, vj0, vj1);
  *V_0 = vui8matrix(*vi0,*vi1, *vj0, *vj1);

  vuint8 initial_V = init_vuint8(V_MIN);
  for(int i = *vi0; i <= *vi1; i++){
    for(int j = *vj0; j <= *vj1; j++){
      vec_store(&(*V_0)[i][j], initial_V);
    }
  }
}



vuint8** SigmaDelta_1step_SIMD(vuint8** M_t_moins_1, vuint8*** M_t_save, vuint8** V_t_moins_1, vuint8*** V_t_save, vuint8** I_t, int vi0, int vi1, int vj0, int vj1)
{
  vuint8** M_t = vui8matrix(vi0, vi1, vj0, vj1);
  vuint8** V_t = vui8matrix(vi0, vi1, vj0, vj1);
  vuint8 O_t, N_O_t;
  vuint8** E_t = vui8matrix(vi0, vi1, vj0, vj1);
  vuint8 vMAX = init_vuint8(V_MAX);
  vuint8 vMIN = init_vuint8(V_MIN);
  vuint8 curr_m_t_moins_1, curr_m_t, curr_i_t, curr_v_t_moins_1, curr_v_t;

  for(int i = vi0; i <= vi1; i++){
    for(int j = vj0; j <= vj1; j++){

      curr_m_t_moins_1 = vec_load(&M_t_moins_1[i][j]);
      curr_v_t_moins_1 = vec_load(&V_t_moins_1[i][j]);
      curr_i_t = vec_load(&I_t[i][j]);

      curr_m_t = vec_cmp_pixels_SD(curr_m_t_moins_1, curr_i_t);
      O_t = vABS_DIFF(curr_m_t, curr_i_t);
      N_O_t = vec_muls_epi8_const(O_t, N);

      curr_v_t = vec_cmp_pixels_SD(curr_v_t_moins_1, N_O_t);
      curr_v_t = vec_MAX(vec_MIN(curr_v_t, vMAX), vMIN);

      vec_store(&M_t[i][j], curr_m_t);
      vec_store(&V_t[i][j], curr_v_t);
      vec_store(&E_t[i][j], vec_cmpgte_vui8(O_t, curr_v_t));
    }
  }

  *M_t_save = M_t;
  *V_t_save = V_t;
  return E_t;
}

void compute_sd_SIMD_all_steps(int save)
{
  if(save)
    CHECK_ERROR(system("mkdir -p output/sd_SIMD"));

  vuint8** M_t_moins_1 = NULL;
  vuint8** V_t_moins_1 = NULL;
  vuint8** E_t = NULL;
  vuint8** I_t = NULL;
  vuint8** M_t = NULL;
  vuint8** V_t = NULL;
  int si0, si1, sj0, sj1;
  int vi0, vi1, vj0, vj1;
  char buff[35];
  SigmaDelta_step0_SIMD(&M_t_moins_1, &V_t_moins_1, &vi0, &vi1, &vj0, &vj1);

  for(int i = 1; i < 200; i++){
    sprintf(buff, "car3/car_3%.3d.pgm", i);
    I_t = LoadPGM_vui8matrix(buff, &si0, &si1, &sj0, &sj1, &vi0, &vi1, &vj0, &vj1);
    E_t = SigmaDelta_1step_SIMD(M_t_moins_1, &M_t, V_t_moins_1, &V_t, I_t, vi0, vi1, vj0, vj1);
    if(save){
      sprintf(buff, "output/sd_SIMD/E_t_%.3d.pgm", i);
      SavePGM_ui8matrix((uint8**)E_t, si0, si1, sj0, sj1, buff);
    }
    free_vui8matrix(M_t_moins_1, vi0, vi1, vj0, vj1);
    free_vui8matrix(V_t_moins_1, vi0, vi1, vj0, vj1);
    M_t_moins_1 = M_t;
    V_t_moins_1 = V_t;
    free_vui8matrix(I_t, vi0, vi1, vj0, vj1);
    free_vui8matrix(E_t, vi0, vi1, vj0, vj1);
  }
  free_vui8matrix(M_t, vi0, vi1, vj0, vj1);
  free_vui8matrix(V_t, vi0, vi1, vj0, vj1);
}
