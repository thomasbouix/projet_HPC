#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nrdef.h"
#include "utils.h"

// Perform convertion between logical and binary convert_coding
// if from = 255 and to = 1 --> conversion from logical to binary coding
// if from = 1 and to = 255 --> conversion from binary to logical coding
void convert_coding(uint8** img, int nrl, int nrh, int ncl, int nch, int from, int to){
  for(int i = nrl; i <= nrh; i++){
    for(int j = ncl; j <= nch; j++){
      if(img[i][j] == from)
        img[i][j] = to;
    }
  }
}
