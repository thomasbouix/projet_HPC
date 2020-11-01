#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nrutil.h"
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
      // O_t[i][j] = (uint8)abs(I_t[i][j] - I_t_moins_1[i][j]);
    }
  }
  return E_t;
}


void compute_fd(int num_image, int threshold){
  int nrl1, nrh1, ncl1, nch1, nrl2, nrh2, ncl2, nch2;
  char buff[30];

  //strcpy(path_image_1, base_path);
  //strcpy(path_image_2, base_path);

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
