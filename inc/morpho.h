#ifndef MORPHO_H
#define MORPHO_H

#define IMG_ROWS 239
#define IMG_COLS 319

uint8** LoadPGM_padding_ui8matrix(char *filename, int* nrl, int* nrh, int* ncl, int* nch, int padding);
void free_padding_ui8matrix(uint8** img_with_padding, int nrl, int nrh, int ncl, int nch, int padding);
uint8** erosion(uint8** img_with_padding, int height, int width, int kernel_size);
uint8** dilatation(uint8** img_with_padding, int height, int width, int kernel_size);
void compute_dilatation(char* basePath, int kernel_size, int save);
void compute_erosion(char* basePath, int kernel_size, int save);

#endif
