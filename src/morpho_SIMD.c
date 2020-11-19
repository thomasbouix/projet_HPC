#include "morpho_SIMD.h"

// AND logique sur un voisinage de taille 3x3
vbits** erosion_3x3_SIMD(vbits** img_bin, int height, int width)
{
  int nb_vbits_col = ceil((float)width/128);
  int nb_unused_col = (128-(width%128))%128;
  vbits **m = (vbits**)vui32matrix(0, height-1, 0, nb_vbits_col-1);

  // Ajout d'un pointeur vers les lignes 0 et height-1 de l'image pour gérer plus facilement les bords
  vbits **img_bin_extra_lines=(vbits **) _mm_malloc ((size_t)((height+2)*sizeof(vbits*)), 16);
  if (!img_bin_extra_lines) vnrerror("allocation failure in erosion_3x3_SIMD()");
  img_bin_extra_lines++;

  for(int i = 0; i < height; i++)
    img_bin_extra_lines[i] = img_bin[i];

  img_bin_extra_lines[-1] = img_bin_extra_lines[0];
  img_bin_extra_lines[height] = img_bin_extra_lines[height-1];

  vbits a0, b0, c0, a0_last, b0_last, c0_last;
  vbits a1, b1, c1, a1_last, b1_last, c1_last;;
  vbits a2, b2, c2, a2_last, b2_last, c2_last;;

  vbits aa0, cc0, aa0_last, cc0_last;
  vbits aa1, cc1, aa1_last, cc1_last;
  vbits aa2, cc2, aa2_last, cc2_last;

  vbits and0, and1, and2, and0_last, and1_last, and2_last;

  vbits y, y_last;
  int n = height%3;

  // Cas particulier des images à moins de 128 colonnes
  if(nb_vbits_col == 1){

    // PROLOGUE
    b0 = vec_load(&img_bin_extra_lines[-1][0]);
    b1 = vec_load(&img_bin_extra_lines[0][0]);

    a0 = _mm_bitshift_right(b0, 127);
    a1 = _mm_bitshift_right(b1, 127);

    c0 = _mm_bitshift_left(b0, 127-nb_unused_col);
    c1 = _mm_bitshift_left(b1, 127-nb_unused_col);

    aa0 = vec_left1_bin(a0, b0);
    cc0 = vec_right1_bin_unused_col(b0, c0, nb_unused_col);
    aa1 = vec_left1_bin(a1, b1);
    cc1 = vec_right1_bin_unused_col(b1, c1, nb_unused_col);

    and0 = vAND3(aa0, b0, cc0);
    and1 = vAND3(aa1, b1, cc1);

    for(int i=0; i < height-n; i+=3){

      b2 = vec_load(&img_bin_extra_lines[i+1][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
      and2 = vAND3(aa2, b2, cc2);
      y = vAND3(and0, and1, and2);
      vec_store(&m[i][0], y);

      b2 = vec_load(&img_bin_extra_lines[i+2][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
      and0 = vAND3(aa2, b2, cc2);
      y = vAND3(and0, and1, and2);
      vec_store(&m[i+1][0], y);

      b2 = vec_load(&img_bin_extra_lines[i+3][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
      and1 = vAND3(aa2, b2, cc2);
      y = vAND3(and0, and1, and2);
      vec_store(&m[i+2][0], y);
    }

    // EPILOGUE
    switch (n) {
      case 2:
        b2 = vec_load(&img_bin_extra_lines[height-1][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
        and2 = vAND3(aa2, b2, cc2);
        y = vAND3(and0, and1, and2);
        vec_store(&m[height-2][0], y);

        b2 = vec_load(&img_bin_extra_lines[height][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
        and0 = vAND3(aa2, b2, cc2);
        y = vAND3(and0, and1, and2);
        vec_store(&m[height-1][0], y);
        break;
      case 1:
        b2 = vec_load(&img_bin_extra_lines[height][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
        and2 = vAND3(aa2, b2, cc2);
        y = vAND3(and0, and1, and2);
        vec_store(&m[height-1][0], y);
        break;
      default:
        break;
    }

    _mm_free(img_bin_extra_lines-1);
    return m;
  }



  // Gestion des cas 'standards'
  a0 = vec_load(&img_bin_extra_lines[-1][0]);
  b0 = vec_load(&img_bin_extra_lines[-1][1]);
  a1 = vec_load(&img_bin_extra_lines[0][0]);
  b1 = vec_load(&img_bin_extra_lines[0][1]);

  for(int j = 1; j < nb_vbits_col-1; j++){
    // PROLOGUE
    c0 = vec_load(&img_bin_extra_lines[-1][j+1]);
    c1 = vec_load(&img_bin_extra_lines[0][j+1]);

    aa0 = vec_left1_bin(a0, b0);
    cc0 = vec_right1_bin(b0, c0);
    aa1 = vec_left1_bin(a1, b1);
    cc1 = vec_right1_bin(b1, c1);

    and0 = vAND3(aa0, b0, cc0);
    and1 = vAND3(aa1, b1, cc1);

    for(int i = 0; i < height-n; i+=3){
      a2 = vec_load(&img_bin_extra_lines[i+1][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+1][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+1][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and2 = vAND3(aa2, b2, cc2);

      y = vAND3(and0, and1, and2);
      vec_store(&m[i][j], y);

      a2 = vec_load(&img_bin_extra_lines[i+2][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+2][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+2][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and0 = vAND3(aa2, b2, cc2);

      y = vAND3(and0, and1, and2);
      vec_store(&m[i+1][j], y);

      a2 = vec_load(&img_bin_extra_lines[i+3][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+3][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+3][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and1 = vAND3(aa2, b2, cc2);

      y = vAND3(and0, and1, and2);
      vec_store(&m[i+2][j], y);
    }
    // EPILOGUE
    switch (n) {
      case 2:
        a2 = vec_load(&img_bin_extra_lines[height-1][j-1]);
        b2 = vec_load(&img_bin_extra_lines[height-1][j+0]);
        c2 = vec_load(&img_bin_extra_lines[height-1][j+1]);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);
        and2 = vAND3(aa2, b2, cc2);
        y = vAND3(and0, and1, and2);
        vec_store(&m[height-2][j], y);

        a2 = vec_load(&img_bin_extra_lines[height][j-1]);
        b2 = vec_load(&img_bin_extra_lines[height][j+0]);
        c2 = vec_load(&img_bin_extra_lines[height][j+1]);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);
        and0 = vAND3(aa2, b2, cc2);
        y = vAND3(and0, and1, and2);
        vec_store(&m[height-1][j], y);
        break;
      case 1:
        a2 = vec_load(&img_bin_extra_lines[height][j-1]);
        b2 = vec_load(&img_bin_extra_lines[height][j+0]);
        c2 = vec_load(&img_bin_extra_lines[height][j+1]);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);
        and2 = vAND3(aa2, b2, cc2);
        y = vAND3(and0, and1, and2);
        vec_store(&m[height-1][j], y);
        break;
      default:
        break;
    }
    a0 = b0; b0 = c0;
    a1 = b1; b1 = c1;
  }

  // Gestion des bords (première et dernière colonnes)
  // PROLOGUE LAST COL
  b0_last = b0;
  b1_last = b1;
  a0_last = a0;
  a1_last = a1;
  c0_last = _mm_bitshift_left(b0_last, 127-nb_unused_col);
  c1_last = _mm_bitshift_left(b1_last, 127-nb_unused_col);

  aa0_last = vec_left1_bin(a0_last, b0_last);
  cc0_last = vec_right1_bin_unused_col(b0_last, c0_last, nb_unused_col);
  aa1_last = vec_left1_bin(a1_last, b1_last);
  cc1_last = vec_right1_bin_unused_col(b1_last, c1_last, nb_unused_col);

  and0_last = vAND3(aa0_last, b0_last, cc0_last);
  and1_last = vAND3(aa1_last, b1_last, cc1_last);

  // PROLOGUE FIRST COL
  b0 = vec_load(&img_bin_extra_lines[-1][0]);
  c0 = vec_load(&img_bin_extra_lines[-1][1]);
  a0 = _mm_bitshift_right(b0, 127);

  b1 = vec_load(&img_bin_extra_lines[0][0]);
  c1 = vec_load(&img_bin_extra_lines[0][1]);
  a1 = _mm_bitshift_right(b1, 127);

  aa0 = vec_left1_bin(a0, b0);
  cc0 = vec_right1_bin(b0, c0);
  aa1 = vec_left1_bin(a1, b1);
  cc1 = vec_right1_bin(b1, c1);

  and0 = vAND3(aa0, b0, cc0);
  and1 = vAND3(aa1, b1, cc1);

  for(int i = 0; i < height-n; i+=3){
    // Déroulage de boucle i+0
    b2 = vec_load(&img_bin_extra_lines[i+1][0]);
    c2 = vec_load(&img_bin_extra_lines[i+1][1]);
    a2 = _mm_bitshift_right(b2, 127);
    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);
    and2 = vAND3(aa2, b2, cc2);
    y = vAND3(and0, and1, and2);
    vec_store(&m[i][0], y);

    b2_last = vec_load(&img_bin_extra_lines[i+1][nb_vbits_col-1]);
    a2_last = vec_load(&img_bin_extra_lines[i+1][nb_vbits_col-2]);
    c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
    aa2_last = vec_left1_bin(a2_last,b2_last);
    cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
    and2_last = vAND3(aa2_last, b2_last, cc2_last);
    y_last = vAND3(and0_last, and1_last, and2_last);
    vec_store(&m[i][nb_vbits_col-1], y_last);

    // Déroulage de boucle i+1
    b2 = vec_load(&img_bin_extra_lines[i+2][0]);
    c2 = vec_load(&img_bin_extra_lines[i+2][1]);
    a2 = _mm_bitshift_right(b2, 127);
    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);
    and0 = vAND3(aa2, b2, cc2);
    y = vAND3(and0, and1, and2);
    vec_store(&m[i+1][0], y);

    b2_last = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-1]);
    a2_last = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-2]);
    c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
    aa2_last = vec_left1_bin(a2_last,b2_last);
    cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
    and0_last = vAND3(aa2_last, b2_last, cc2_last);
    y_last = vAND3(and0_last, and1_last, and2_last);
    vec_store(&m[i+1][nb_vbits_col-1], y_last);

    // Déroulage de boucle i+2
    b2 = vec_load(&img_bin_extra_lines[i+3][0]);
    c2 = vec_load(&img_bin_extra_lines[i+3][1]);
    a2 = _mm_bitshift_right(b2, 127);
    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);
    and1 = vAND3(aa2, b2, cc2);
    y = vAND3(and0, and1, and2);
    vec_store(&m[i+2][0], y);

    b2_last = vec_load(&img_bin_extra_lines[i+3][nb_vbits_col-1]);
    a2_last = vec_load(&img_bin_extra_lines[i+3][nb_vbits_col-2]);
    c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
    aa2_last = vec_left1_bin(a2_last,b2_last);
    cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
    and1_last = vAND3(aa2_last, b2_last, cc2_last);
    y_last = vAND3(and0_last, and1_last, and2_last);
    vec_store(&m[i+2][nb_vbits_col-1], y_last);
  }

  // EPILOGUE
  switch (n) {
    case 2:
      b2 = vec_load(&img_bin_extra_lines[height-1][0]);
      c2 = vec_load(&img_bin_extra_lines[height-1][1]);
      a2 = _mm_bitshift_right(b2, 127);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and2 = vAND3(aa2, b2, cc2);
      y = vAND3(and0, and1, and2);
      vec_store(&m[height-2][0], y);

      b2_last = vec_load(&img_bin_extra_lines[height-1][nb_vbits_col-1]);
      a2_last = vec_load(&img_bin_extra_lines[height-1][nb_vbits_col-2]);
      c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
      aa2_last = vec_left1_bin(a2_last,b2_last);
      cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
      and2_last = vAND3(aa2_last, b2_last, cc2_last);
      y_last = vAND3(and0_last, and1_last, and2_last);
      vec_store(&m[height-2][nb_vbits_col-1], y_last);


      b2 = vec_load(&img_bin_extra_lines[height][0]);
      c2 = vec_load(&img_bin_extra_lines[height][1]);
      a2 = _mm_bitshift_right(b2, 127);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and0 = vAND3(aa2, b2, cc2);
      y = vAND3(and0, and1, and2);
      vec_store(&m[height-1][0], y);

      b2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-1]);
      a2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-2]);
      c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
      aa2_last = vec_left1_bin(a2_last,b2_last);
      cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
      and0_last = vAND3(aa2_last, b2_last, cc2_last);
      y_last = vAND3(and0_last, and1_last, and2_last);
      vec_store(&m[height-1][nb_vbits_col-1], y_last);
      break;
    case 1:
      b2 = vec_load(&img_bin_extra_lines[height][0]);
      c2 = vec_load(&img_bin_extra_lines[height][1]);
      a2 = _mm_bitshift_right(b2, 127);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and2 = vAND3(aa2, b2, cc2);
      y = vAND3(and0, and1, and2);
      vec_store(&m[height-1][0], y);

      b2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-1]);
      a2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-2]);
      c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
      aa2_last = vec_left1_bin(a2_last,b2_last);
      cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
      and2_last = vAND3(aa2_last, b2_last, cc2_last);
      y_last = vAND3(and0_last, and1_last, and2_last);
      vec_store(&m[height-1][nb_vbits_col-1], y_last);
      break;
    default:
      break;
  }

  _mm_free(img_bin_extra_lines-1);
  return m;
}


void compute_all_erosion_3x3_SIMD(char* basePath, int save)
{
  CHECK_ERROR(system("mkdir -p output/erosion_3x3_SIMD"));
  char buff[40];
  int nrl, nrh, ncl, nch;
  size_t height, width;
  uint8 **img,  **output;
  vbits **img_bin, **output_bin;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_ui8matrix(buff, &nrl, &nrh, &ncl, &nch);
    height = nrh-nrl+1;
    width = nch-ncl+1;

    img_bin = convert_to_binary(img, height, width);
    output_bin = erosion_3x3_SIMD(img_bin, height, width);
    output = convert_from_binary(output_bin, height, width);
    if(save){
      sprintf(buff, "output/erosion_3x3_SIMD/ero3%.3d.pgm", i);
      SavePGM_ui8matrix(output, nrl, nrh, ncl, nch, buff);
    }
    free_ui8matrix(img, nrl, nrh, ncl, nch);
    free_ui8matrix(output, nrl, nrh, ncl, nch);
    free_vbitsmatrix(img_bin, height, width);
    free_vbitsmatrix(output_bin, height, width);
  }
}





// OR logique sur un voisinage de taille 3x3
vbits** dilatation_3x3_SIMD(vbits** img_bin, int height, int width)
{
  int nb_vbits_col = ceil((float)width/128);
  int nb_unused_col = (128-(width%128))%128;
  vbits **m = (vbits**)vui32matrix(0, height-1, 0, nb_vbits_col-1);

  // Ajout d'un pointeur vers les lignes 0 et height-1 de l'image pour gérer plus facilement les bords
  vbits **img_bin_extra_lines=(vbits **) _mm_malloc ((size_t)((height+2)*sizeof(vbits*)), 16);
  if (!img_bin_extra_lines) vnrerror("allocation failure in dilatation_3x3_SIMDS()");
  img_bin_extra_lines++;

  for(int i = 0; i < height; i++)
    img_bin_extra_lines[i] = img_bin[i];

  img_bin_extra_lines[-1] = img_bin_extra_lines[0];
  img_bin_extra_lines[height] = img_bin_extra_lines[height-1];

  vbits a0, b0, c0, a0_last, b0_last, c0_last;
  vbits a1, b1, c1, a1_last, b1_last, c1_last;;
  vbits a2, b2, c2, a2_last, b2_last, c2_last;;

  vbits aa0, cc0, aa0_last, cc0_last;
  vbits aa1, cc1, aa1_last, cc1_last;
  vbits aa2, cc2, aa2_last, cc2_last;

  vbits or0, or1, or2, or0_last, or1_last, or2_last;

  vbits y, y_last;
  int n = height%3;

  // Cas particulier des images à moins de 128 colonnes
  if(nb_vbits_col == 1){

    // PROLOGUE
    b0 = vec_load(&img_bin_extra_lines[-1][0]);
    b1 = vec_load(&img_bin_extra_lines[0][0]);

    a0 = _mm_bitshift_right(b0, 127);
    a1 = _mm_bitshift_right(b1, 127);

    c0 = _mm_bitshift_left(b0, 127-nb_unused_col);
    c1 = _mm_bitshift_left(b1, 127-nb_unused_col);

    aa0 = vec_left1_bin(a0, b0);
    cc0 = vec_right1_bin_unused_col(b0, c0, nb_unused_col);
    aa1 = vec_left1_bin(a1, b1);
    cc1 = vec_right1_bin_unused_col(b1, c1, nb_unused_col);

    or0 = vOR3(aa0, b0, cc0);
    or1 = vOR3(aa1, b1, cc1);

    for(int i=0; i < height-n; i+=3){

      b2 = vec_load(&img_bin_extra_lines[i+1][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
      or2 = vOR3(aa2, b2, cc2);
      y = vOR3(or0, or1, or2);
      vec_store(&m[i][0], y);

      b2 = vec_load(&img_bin_extra_lines[i+2][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
      or0 = vOR3(aa2, b2, cc2);
      y = vOR3(or0, or1, or2);
      vec_store(&m[i+1][0], y);

      b2 = vec_load(&img_bin_extra_lines[i+3][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
      or1 = vOR3(aa2, b2, cc2);
      y = vOR3(or0, or1, or2);
      vec_store(&m[i+2][0], y);
    }

    // EPILOGUE
    switch (n) {
      case 2:
        b2 = vec_load(&img_bin_extra_lines[height-1][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
        or2 = vOR3(aa2, b2, cc2);
        y = vOR3(or0, or1, or2);
        vec_store(&m[height-2][0], y);

        b2 = vec_load(&img_bin_extra_lines[height][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
        or0 = vOR3(aa2, b2, cc2);
        y = vOR3(or0, or1, or2);
        vec_store(&m[height-1][0], y);
        break;
      case 1:
        b2 = vec_load(&img_bin_extra_lines[height][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);
        or2 = vOR3(aa2, b2, cc2);
        y = vOR3(or0, or1, or2);
        vec_store(&m[height-1][0], y);
        break;
      default:
        break;
    }

    _mm_free(img_bin_extra_lines-1);
    return m;
  }


  // Gestion des cas 'standards'
  a0 = vec_load(&img_bin_extra_lines[-1][0]);
  b0 = vec_load(&img_bin_extra_lines[-1][1]);
  a1 = vec_load(&img_bin_extra_lines[0][0]);
  b1 = vec_load(&img_bin_extra_lines[0][1]);

  for(int j = 1; j < nb_vbits_col-1; j++){
    // PROLOGUE

    c0 = vec_load(&img_bin_extra_lines[-1][j+1]);
    c1 = vec_load(&img_bin_extra_lines[0][j+1]);

    aa0 = vec_left1_bin(a0, b0);
    cc0 = vec_right1_bin(b0, c0);
    aa1 = vec_left1_bin(a1, b1);
    cc1 = vec_right1_bin(b1, c1);

    or0 = vOR3(aa0, b0, cc0);
    or1 = vOR3(aa1, b1, cc1);

    for(int i = 0; i < height-n; i+=3){
      a2 = vec_load(&img_bin_extra_lines[i+1][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+1][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+1][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      or2 = vOR3(aa2, b2, cc2);

      y = vOR3(or0, or1, or2);
      vec_store(&m[i][j], y);

      a2 = vec_load(&img_bin_extra_lines[i+2][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+2][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+2][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      or0 = vOR3(aa2, b2, cc2);

      y = vOR3(or0, or1, or2);
      vec_store(&m[i+1][j], y);

      a2 = vec_load(&img_bin_extra_lines[i+3][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+3][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+3][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      or1 = vOR3(aa2, b2, cc2);

      y = vOR3(or0, or1, or2);
      vec_store(&m[i+2][j], y);
    }
    // EPILOGUE
    switch (n) {
      case 2:
        a2 = vec_load(&img_bin_extra_lines[height-1][j-1]);
        b2 = vec_load(&img_bin_extra_lines[height-1][j+0]);
        c2 = vec_load(&img_bin_extra_lines[height-1][j+1]);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);
        or2 = vOR3(aa2, b2, cc2);
        y = vOR3(or0, or1, or2);
        vec_store(&m[height-2][j], y);

        a2 = vec_load(&img_bin_extra_lines[height][j-1]);
        b2 = vec_load(&img_bin_extra_lines[height][j+0]);
        c2 = vec_load(&img_bin_extra_lines[height][j+1]);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);
        or0 = vOR3(aa2, b2, cc2);
        y = vOR3(or0, or1, or2);
        vec_store(&m[height-1][j], y);
        break;
      case 1:
        a2 = vec_load(&img_bin_extra_lines[height][j-1]);
        b2 = vec_load(&img_bin_extra_lines[height][j+0]);
        c2 = vec_load(&img_bin_extra_lines[height][j+1]);
        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);
        or2 = vOR3(aa2, b2, cc2);
        y = vOR3(or0, or1, or2);
        vec_store(&m[height-1][j], y);
        break;
      default:
        break;
    }
    a0 = b0; b0 = c0;
    a1 = b1; b1 = c1;
  }

  // Gestion des bords (première et dernière colonnes)
  // PROLOGUE LAST COL
  b0_last = b0;
  b1_last = b1;
  a0_last = a0;
  a1_last = a1;
  c0_last = _mm_bitshift_left(b0_last, 127-nb_unused_col);
  c1_last = _mm_bitshift_left(b1_last, 127-nb_unused_col);

  aa0_last = vec_left1_bin(a0_last, b0_last);
  cc0_last = vec_right1_bin_unused_col(b0_last, c0_last, nb_unused_col);
  aa1_last = vec_left1_bin(a1_last, b1_last);
  cc1_last = vec_right1_bin_unused_col(b1_last, c1_last, nb_unused_col);

  or0_last = vOR3(aa0_last, b0_last, cc0_last);
  or1_last = vOR3(aa1_last, b1_last, cc1_last);

  // PROLOGUE FIRST COL
  b0 = vec_load(&img_bin_extra_lines[-1][0]);
  c0 = vec_load(&img_bin_extra_lines[-1][1]);
  a0 = _mm_bitshift_right(b0, 127);

  b1 = vec_load(&img_bin_extra_lines[0][0]);
  c1 = vec_load(&img_bin_extra_lines[0][1]);
  a1 = _mm_bitshift_right(b1, 127);

  aa0 = vec_left1_bin(a0, b0);
  cc0 = vec_right1_bin(b0, c0);
  aa1 = vec_left1_bin(a1, b1);
  cc1 = vec_right1_bin(b1, c1);

  or0 = vOR3(aa0, b0, cc0);
  or1 = vOR3(aa1, b1, cc1);


  for(int i = 0; i < height-n; i+=3){
    // Déroulage de boucle i+0
    b2 = vec_load(&img_bin_extra_lines[i+1][0]);
    c2 = vec_load(&img_bin_extra_lines[i+1][1]);
    a2 = _mm_bitshift_right(b2, 127);
    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);
    or2 = vOR3(aa2, b2, cc2);
    y = vOR3(or0, or1, or2);
    vec_store(&m[i][0], y);

    b2_last = vec_load(&img_bin_extra_lines[i+1][nb_vbits_col-1]);
    a2_last = vec_load(&img_bin_extra_lines[i+1][nb_vbits_col-2]);
    c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
    aa2_last = vec_left1_bin(a2_last,b2_last);
    cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
    or2_last = vOR3(aa2_last, b2_last, cc2_last);
    y_last = vOR3(or0_last, or1_last, or2_last);
    vec_store(&m[i][nb_vbits_col-1], y_last);

    // Déroulage de boucle i+1
    b2 = vec_load(&img_bin_extra_lines[i+2][0]);
    c2 = vec_load(&img_bin_extra_lines[i+2][1]);
    a2 = _mm_bitshift_right(b2, 127);
    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);
    or0 = vOR3(aa2, b2, cc2);
    y = vOR3(or0, or1, or2);
    vec_store(&m[i+1][0], y);

    b2_last = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-1]);
    a2_last = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-2]);
    c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
    aa2_last = vec_left1_bin(a2_last,b2_last);
    cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
    or0_last = vOR3(aa2_last, b2_last, cc2_last);
    y_last = vOR3(or0_last, or1_last, or2_last);
    vec_store(&m[i+1][nb_vbits_col-1], y_last);

    // Déroulage de boucle i+2
    b2 = vec_load(&img_bin_extra_lines[i+3][0]);
    c2 = vec_load(&img_bin_extra_lines[i+3][1]);
    a2 = _mm_bitshift_right(b2, 127);
    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);
    or1 = vOR3(aa2, b2, cc2);
    y = vOR3(or0, or1, or2);
    vec_store(&m[i+2][0], y);

    b2_last = vec_load(&img_bin_extra_lines[i+3][nb_vbits_col-1]);
    a2_last = vec_load(&img_bin_extra_lines[i+3][nb_vbits_col-2]);
    c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
    aa2_last = vec_left1_bin(a2_last,b2_last);
    cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
    or1_last = vOR3(aa2_last, b2_last, cc2_last);
    y_last = vOR3(or0_last, or1_last, or2_last);
    vec_store(&m[i+2][nb_vbits_col-1], y_last);
  }

  // EPILOGUE
  switch (n) {
    case 2:
      b2 = vec_load(&img_bin_extra_lines[height-1][0]);
      c2 = vec_load(&img_bin_extra_lines[height-1][1]);
      a2 = _mm_bitshift_right(b2, 127);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      or2 = vOR3(aa2, b2, cc2);
      y = vOR3(or0, or1, or2);
      vec_store(&m[height-2][0], y);

      b2_last = vec_load(&img_bin_extra_lines[height-1][nb_vbits_col-1]);
      a2_last = vec_load(&img_bin_extra_lines[height-1][nb_vbits_col-2]);
      c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
      aa2_last = vec_left1_bin(a2_last,b2_last);
      cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
      or2_last = vOR3(aa2_last, b2_last, cc2_last);
      y_last = vOR3(or0_last, or1_last, or2_last);
      vec_store(&m[height-2][nb_vbits_col-1], y_last);


      b2 = vec_load(&img_bin_extra_lines[height][0]);
      c2 = vec_load(&img_bin_extra_lines[height][1]);
      a2 = _mm_bitshift_right(b2, 127);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      or0 = vOR3(aa2, b2, cc2);
      y = vOR3(or0, or1, or2);
      vec_store(&m[height-1][0], y);

      b2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-1]);
      a2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-2]);
      c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
      aa2_last = vec_left1_bin(a2_last,b2_last);
      cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
      or0_last = vOR3(aa2_last, b2_last, cc2_last);
      y_last = vOR3(or0_last, or1_last, or2_last);
      vec_store(&m[height-1][nb_vbits_col-1], y_last);
      break;
    case 1:
      b2 = vec_load(&img_bin_extra_lines[height][0]);
      c2 = vec_load(&img_bin_extra_lines[height][1]);
      a2 = _mm_bitshift_right(b2, 127);
      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      or2 = vOR3(aa2, b2, cc2);
      y = vOR3(or0, or1, or2);
      vec_store(&m[height-1][0], y);

      b2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-1]);
      a2_last = vec_load(&img_bin_extra_lines[height][nb_vbits_col-2]);
      c2_last = _mm_bitshift_left(b2_last, 127-nb_unused_col);
      aa2_last = vec_left1_bin(a2_last,b2_last);
      cc2_last = vec_right1_bin_unused_col(b2_last,c2_last,nb_unused_col);
      or2_last = vOR3(aa2_last, b2_last, cc2_last);
      y_last = vOR3(or0_last, or1_last, or2_last);
      vec_store(&m[height-1][nb_vbits_col-1], y_last);
      break;
    default:
      break;
  }

  _mm_free(img_bin_extra_lines-1);
  return m;
}


void compute_all_dilatation_3x3_SIMD(char* basePath, int save)
{
  CHECK_ERROR(system("mkdir -p output/dilatation_3x3_SIMD"));
  char buff[40];
  int nrl, nrh, ncl, nch;
  size_t height, width;
  uint8 **img,  **output;
  vbits **img_bin, **output_bin;

  for(int i = 1; i < 200; i++){
    sprintf(buff, "%s%.3d.pgm", basePath, i);
    img = LoadPGM_ui8matrix(buff, &nrl, &nrh, &ncl, &nch);
    height = nrh-nrl+1;
    width = nch-ncl+1;

    img_bin = convert_to_binary(img, height, width);
    output_bin = dilatation_3x3_SIMD(img_bin, height, width);
    output = convert_from_binary(output_bin, height, width);
    if(save){
      sprintf(buff, "output/dilatation_3x3_SIMD/dil3%.3d.pgm", i);
      SavePGM_ui8matrix(output, nrl, nrh, ncl, nch, buff);
    }
    free_ui8matrix(img, nrl, nrh, ncl, nch);
    free_ui8matrix(output, nrl, nrh, ncl, nch);
    free_vbitsmatrix(img_bin, height, width);
    free_vbitsmatrix(output_bin, height, width);
  }
}





vbits** ouverture_SIMD(vbits** img_bin, int height, int width)
{
  int nb_vbits_col = ceil((float)width/128);
  int nb_unused_col = (128-(width%128))%128;
  int n = height%3;

  vbits **m = (vbits**)vui32matrix(0, height-1, 0, nb_vbits_col-1);

  // Ajout d'un pointeur vers les lignes 0 et height-1 de l'image pour gérer plus facilement les bords
  vbits **img_bin_extra_lines=(vbits **) _mm_malloc ((size_t)((height+2)*sizeof(vbits*)), 16);
  if (!img_bin_extra_lines) vnrerror("allocation failure in ouverture_SIMD()");
  img_bin_extra_lines++;

  for(int i = 0; i < height; i++)
    img_bin_extra_lines[i] = img_bin[i];

  img_bin_extra_lines[-1] = img_bin_extra_lines[0];
  img_bin_extra_lines[height] = img_bin_extra_lines[height-1];

  vbits a0, a1, a2;
  vbits b0, b1, b2;
  vbits c0, c1, c2;
  vbits aa0, aa1, aa2;
  vbits cc0, cc1, cc2;
  vbits or0, or1, or2, or0_bis, or1_bis, or2_bis, or0_ter, or1_ter, or2_ter;
  vbits and0, and1, and2, and3, and4, and5, and6, and7, and8;
  vbits and0_0, and0_1, and0_2, and1_0, and1_1, and1_2, and2_0, and2_1, and2_2;
  vbits y;


  // CAS DES IMAGES AYANT MOINS DE 128 COLONNES
  if(nb_vbits_col == 1){
    b0 = vec_load(&img_bin_extra_lines[-1][0]);
    b1 = vec_load(&img_bin_extra_lines[0][0]);
    b2 = vec_load(&img_bin_extra_lines[1][0]);

    a0 = _mm_bitshift_right(b0, 127);
    a1 = _mm_bitshift_right(b1, 127);
    a2 = _mm_bitshift_right(b2, 127);

    c0 = _mm_bitshift_left(b0, 127-nb_unused_col);
    c1 = _mm_bitshift_left(b1, 127-nb_unused_col);
    c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

    aa0 = vec_left1_bin(a0,b0);
    aa1 = vec_left1_bin(a1,b1);
    aa2 = vec_left1_bin(a2,b2);

    cc0 = vec_right1_bin(b0,c0);
    cc1 = vec_right1_bin(b1,c1);
    cc2 = vec_right1_bin(b2,c2);

    and0_0 = vAND3(aa0, b0, cc0);
    and0_1 = vAND3(aa1, b1, cc1);
    and0_2 = vAND3(aa2, b2, cc2);

    and0 = vAND3(and0_0, and0_0, and0_1);
    and3 = vAND3(and0_0, and0_1, and0_2);

    aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
    cc0 = vec_right1_bin_unused_col(and0, _mm_bitshift_left(and0, 127-nb_unused_col),nb_unused_col);
    or0 = vOR3(aa0, and0, cc0);

    aa0 = vec_left1_bin(_mm_bitshift_right(and3, 127),and3);
    cc0 = vec_right1_bin_unused_col(and3, _mm_bitshift_left(and3, 127-nb_unused_col),nb_unused_col);
    or1 = vOR3(aa0, and3, cc0);

    for(int i = 0; i < height-n-1; i+=3){
      b2 = vec_load(&img_bin_extra_lines[i+2][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_0 = vAND3(aa2, b2, cc2);
      and6 = vAND3(and0_0, and0_2, and0_1);

      aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
      cc0 = vec_right1_bin_unused_col(and6, _mm_bitshift_left(and6, 127-nb_unused_col),nb_unused_col);
      or2 = vOR3(aa0, and6, cc0);

      y = vOR3(or0, or1, or2);
      vec_store(&m[i][0], y);

      b2 = vec_load(&img_bin_extra_lines[i+3][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_1 = vAND3(aa2, b2, cc2);
      and0 = vAND3(and0_1, and0_0, and0_2);

      aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
      cc0 = vec_right1_bin_unused_col(and0, _mm_bitshift_left(and0, 127-nb_unused_col),nb_unused_col);
      or0 = vOR3(aa0, and0, cc0);

      y = vOR3(or0, or2, or1);
      vec_store(&m[i+1][0], y);

      b2 = vec_load(&img_bin_extra_lines[i+4][0]);
      a2 = _mm_bitshift_right(b2, 127);
      c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_2 = vAND3(aa2, b2, cc2);
      and3 = vAND3(and0_2, and0_1, and0_0);

      aa0 = vec_left1_bin(_mm_bitshift_right(and3, 127),and3);
      cc0 = vec_right1_bin_unused_col(and3, _mm_bitshift_left(and3, 127-nb_unused_col),nb_unused_col);
      or1 = vOR3(aa0, and3, cc0);

      y = vOR3(or1, or0, or2);
      vec_store(&m[i+2][0], y);
    }

    switch(n){
      case 2 :
        b2 = vec_load(&img_bin_extra_lines[height-1][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_0 = vAND3(aa2, b2, cc2);
        and6 = vAND3(and0_0, and0_2, and0_1);

        aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
        cc0 = vec_right1_bin_unused_col(and6, _mm_bitshift_left(and6, 127-nb_unused_col),nb_unused_col);
        or2 = vOR3(aa0, and6, cc0);

        y = vOR3(or0, or1, or2);
        vec_store(&m[height-3][0], y);

        b2 = vec_load(&img_bin_extra_lines[height][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_1 = vAND3(aa2, b2, cc2);
        and0 = vAND3(and0_1, and0_0, and0_2);

        aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
        cc0 = vec_right1_bin_unused_col(and0, _mm_bitshift_left(and0, 127-nb_unused_col),nb_unused_col);
        or0 = vOR3(aa0, and0, cc0);

        y = vOR3(or0, or2, or1);
        vec_store(&m[height-2][0], y);

        y = vOR3(or2, or0, or0);
        vec_store(&m[height-1][0], y);

      break;
      case 1 :
        b2 = vec_load(&img_bin_extra_lines[height][0]);
        a2 = _mm_bitshift_right(b2, 127);
        c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_0 = vAND3(aa2, b2, cc2);
        and6 = vAND3(and0_0, and0_2, and0_1);

        aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
        cc0 = vec_right1_bin_unused_col(and6, _mm_bitshift_left(and6, 127-nb_unused_col),nb_unused_col);
        or2 = vOR3(aa0, and6, cc0);

        y = vOR3(or0, or1, or2);
        vec_store(&m[height-2][0], y);

        y = vOR3(or1, or2, or2);
        vec_store(&m[height-1][0], y);
      break;
      case 0 :
        y = vOR3(or0, or1, or1);
        vec_store(&m[height-1][0], y);
      break;
    }

    _mm_free(img_bin_extra_lines-1);
    return m;
  }

  // CAS DES IMAGES ENTRE 1*128+1 ET 2*128 COLONNES
  if(nb_vbits_col == 2){
    b0 = vec_load(&img_bin_extra_lines[-1][0]);
    b1 = vec_load(&img_bin_extra_lines[0][0]);
    b2 = vec_load(&img_bin_extra_lines[1][0]);

    c0 = vec_load(&img_bin_extra_lines[-1][1]);
    c1 = vec_load(&img_bin_extra_lines[0][1]);
    c2 = vec_load(&img_bin_extra_lines[1][1]);

    a0 = _mm_bitshift_right(b0, 127);
    a1 = _mm_bitshift_right(b1, 127);
    a2 = _mm_bitshift_right(b2, 127);

    aa0 = vec_left1_bin(a0,b0);
    aa1 = vec_left1_bin(a1,b1);
    aa2 = vec_left1_bin(a2,b2);

    cc0 = vec_right1_bin(b0,c0);
    cc1 = vec_right1_bin(b1,c1);
    cc2 = vec_right1_bin(b2,c2);

    and0_0 = vAND3(aa0, b0, cc0);
    and0_1 = vAND3(aa1, b1, cc1);
    and0_2 = vAND3(aa2, b2, cc2);

    and0 = vAND3(and0_0, and0_0, and0_1);
    and3 = vAND3(and0_0, and0_1, and0_2);

    a0 = _mm_bitshift_left(c0, 127-nb_unused_col);
    a1 = _mm_bitshift_left(c1, 127-nb_unused_col);
    a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

    aa0 = vec_left1_bin(b0,c0);
    aa1 = vec_left1_bin(b1,c1);
    aa2 = vec_left1_bin(b2,c2);

    cc0 = vec_right1_bin(c0,a0);
    cc1 = vec_right1_bin(c1,a1);
    cc2 = vec_right1_bin(c2,a2);

    and1_0 = vAND3(aa0, c0, cc0);
    and1_1 = vAND3(aa1, c1, cc1);
    and1_2 = vAND3(aa2, c2, cc2);

    and1 = vAND3(and1_0, and1_0, and1_1);
    and4 = vAND3(and1_0, and1_1, and1_2);

    aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
    cc0 = vec_right1_bin(and0,and1);
    or0 = vOR3(aa0, and0, cc0);

    aa0 = vec_left1_bin(and0,and1);
    cc0 = vec_right1_bin_unused_col(and1, _mm_bitshift_left(and1, 127-nb_unused_col),nb_unused_col);
    or0_bis = vOR3(aa0, and1, cc0);

    aa0 = vec_left1_bin(_mm_bitshift_right(and3, 127),and3);
    cc0 = vec_right1_bin(and3,and4);
    or1 = vOR3(aa0, and3, cc0);

    aa0 = vec_left1_bin(and3,and4);
    cc0 = vec_right1_bin_unused_col(and4, _mm_bitshift_left(and4, 127-nb_unused_col),nb_unused_col);
    or1_bis = vOR3(aa0, and4, cc0);

    for(int i = 0; i < height-n-1; i+=3){
      b2 = vec_load(&img_bin_extra_lines[i+2][0]);
      c2 = vec_load(&img_bin_extra_lines[i+2][1]);
      a2 = _mm_bitshift_right(b2, 127);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_0 = vAND3(aa2, b2, cc2);
      and6 = vAND3(and0_0, and0_2, and0_1);

      a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_0 = vAND3(aa2, c2, cc2);
      and7 = vAND3(and1_0, and1_2, and1_1);

      aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
      cc0 = vec_right1_bin(and6,and7);
      or2 = vOR3(aa0, and6, cc0);

      aa0 = vec_left1_bin(and6,and7);
      cc0 = vec_right1_bin_unused_col(and7, _mm_bitshift_left(and7, 127-nb_unused_col),nb_unused_col);
      or2_bis = vOR3(aa0, and7, cc0);

      y = vOR3(or2, or0, or1);
      vec_store(&m[i][0], y);

      y = vOR3(or2_bis, or0_bis, or1_bis);
      vec_store(&m[i][1], y);

      b2 = vec_load(&img_bin_extra_lines[i+3][0]);
      c2 = vec_load(&img_bin_extra_lines[i+3][1]);
      a2 = _mm_bitshift_right(b2, 127);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_1 = vAND3(aa2, b2, cc2);
      and0 = vAND3(and0_1, and0_0, and0_2);

      a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_1 = vAND3(aa2, c2, cc2);
      and1 = vAND3(and1_1, and1_0, and1_2);

      aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
      cc0 = vec_right1_bin(and0,and1);
      or0 = vOR3(aa0, and0, cc0);

      aa0 = vec_left1_bin(and0,and1);
      cc0 = vec_right1_bin_unused_col(and1, _mm_bitshift_left(and1, 127-nb_unused_col),nb_unused_col);
      or0_bis = vOR3(aa0, and1, cc0);

      y = vOR3(or0, or2, or1);
      vec_store(&m[i+1][0], y);

      y = vOR3(or0_bis, or2_bis, or1_bis);
      vec_store(&m[i+1][1], y);

      b2 = vec_load(&img_bin_extra_lines[i+4][0]);
      c2 = vec_load(&img_bin_extra_lines[i+4][1]);
      a2 = _mm_bitshift_right(b2, 127);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_2 = vAND3(aa2, b2, cc2);
      and3 = vAND3(and0_2, and0_1, and0_0);

      a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_2 = vAND3(aa2, c2, cc2);
      and4 = vAND3(and1_2, and1_1, and1_0);

      aa0 = vec_left1_bin(_mm_bitshift_right(and3, 127),and3);
      cc0 = vec_right1_bin(and3,and4);
      or1 = vOR3(aa0, and3, cc0);

      aa0 = vec_left1_bin(and3,and4);
      cc0 = vec_right1_bin_unused_col(and4, _mm_bitshift_left(and4, 127-nb_unused_col),nb_unused_col);
      or1_bis = vOR3(aa0, and4, cc0);

      y = vOR3(or1, or0, or2);
      vec_store(&m[i+2][0], y);

      y = vOR3(or1_bis, or0_bis, or2_bis);
      vec_store(&m[i+2][1], y);

    }

    switch(n){
      case 2 :
        b2 = vec_load(&img_bin_extra_lines[height-1][0]);
        c2 = vec_load(&img_bin_extra_lines[height-1][1]);
        a2 = _mm_bitshift_right(b2, 127);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_0 = vAND3(aa2, b2, cc2);
        and6 = vAND3(and0_0, and0_2, and0_1);

        a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

        aa2 = vec_left1_bin(b2,c2);
        cc2 = vec_right1_bin(c2,a2);

        and1_0 = vAND3(aa2, c2, cc2);
        and7 = vAND3(and1_0, and1_2, and1_1);

        aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
        cc0 = vec_right1_bin(and6,and7);
        or2 = vOR3(aa0, and6, cc0);

        aa0 = vec_left1_bin(and6,and7);
        cc0 = vec_right1_bin_unused_col(and7, _mm_bitshift_left(and7, 127-nb_unused_col),nb_unused_col);
        or2_bis = vOR3(aa0, and7, cc0);

        y = vOR3(or2, or0, or1);
        vec_store(&m[height-3][0], y);

        y = vOR3(or2_bis, or0_bis, or1_bis);
        vec_store(&m[height-3][1], y);

        b2 = vec_load(&img_bin_extra_lines[height][0]);
        c2 = vec_load(&img_bin_extra_lines[height][1]);
        a2 = _mm_bitshift_right(b2, 127);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_1 = vAND3(aa2, b2, cc2);
        and0 = vAND3(and0_1, and0_0, and0_2);

        a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

        aa2 = vec_left1_bin(b2,c2);
        cc2 = vec_right1_bin(c2,a2);

        and1_1 = vAND3(aa2, c2, cc2);
        and1 = vAND3(and1_1, and1_0, and1_2);

        aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
        cc0 = vec_right1_bin(and0,and1);
        or0 = vOR3(aa0, and0, cc0);

        aa0 = vec_left1_bin(and0,and1);
        cc0 = vec_right1_bin_unused_col(and1, _mm_bitshift_left(and1, 127-nb_unused_col),nb_unused_col);
        or0_bis = vOR3(aa0, and1, cc0);

        y = vOR3(or0, or2, or1);
        vec_store(&m[height-2][0], y);

        y = vOR3(or0_bis, or2_bis, or1_bis);
        vec_store(&m[height-2][1], y);

        y = vOR3(or2, or0, or0);
        vec_store(&m[height-1][0], y);
        y = vOR3(or2_bis, or0_bis, or0_bis);
        vec_store(&m[height-1][1], y);

      break;
      case 1 :

        b2 = vec_load(&img_bin_extra_lines[height][0]);
        c2 = vec_load(&img_bin_extra_lines[height][1]);
        a2 = _mm_bitshift_right(b2, 127);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_0 = vAND3(aa2, b2, cc2);
        and6 = vAND3(and0_0, and0_2, and0_1);

        a2 = _mm_bitshift_left(c2, 127-nb_unused_col);

        aa2 = vec_left1_bin(b2,c2);
        cc2 = vec_right1_bin(c2,a2);

        and1_0 = vAND3(aa2, c2, cc2);
        and7 = vAND3(and1_0, and1_2, and1_1);

        aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
        cc0 = vec_right1_bin(and6,and7);
        or2 = vOR3(aa0, and6, cc0);

        aa0 = vec_left1_bin(and6,and7);
        cc0 = vec_right1_bin_unused_col(and7, _mm_bitshift_left(and7, 127-nb_unused_col),nb_unused_col);
        or2_bis = vOR3(aa0, and7, cc0);

        y = vOR3(or2, or0, or1);
        vec_store(&m[height-2][0], y);

        y = vOR3(or2_bis, or0_bis, or1_bis);
        vec_store(&m[height-2][1], y);

        y = vOR3(or1, or2, or2);
        vec_store(&m[height-1][0], y);
        y = vOR3(or1_bis, or2_bis, or2_bis);
        vec_store(&m[height-1][1], y);
      break;
      case 0 :
        y = vOR3(or0, or1, or1);
        vec_store(&m[height-1][0], y);
        y = vOR3(or0_bis, or1_bis, or1_bis);
        vec_store(&m[height-1][1], y);
      break;
      default:
      break;
    }

    _mm_free(img_bin_extra_lines-1);
    return m;
  }








  // CAS DES IMAGES ENTRE 2*128+1 ET 3*128 COLONNES
  if(nb_vbits_col == 3){
    b0 = vec_load(&img_bin_extra_lines[0][0]);
    b1 = vec_load(&img_bin_extra_lines[1][0]);

    c0 = vec_load(&img_bin_extra_lines[0][1]);
    c1 = vec_load(&img_bin_extra_lines[1][1]);

    a0 = _mm_bitshift_right(b0, 127);
    a1 = _mm_bitshift_right(b1, 127);

    aa0 = vec_left1_bin(a0,b0);
    aa1 = vec_left1_bin(a1,b1);

    cc0 = vec_right1_bin(b0,c0);
    cc1 = vec_right1_bin(b1,c1);

    and0_0 = vAND3(aa0, b0, cc0);
    and0_1 = vAND3(aa1, b1, cc1);

    and0 = vAND3(and0_0, and0_0, and0_1);
    and3 = and0;

    a0 = vec_load(&img_bin_extra_lines[0][2]);
    a1 = vec_load(&img_bin_extra_lines[1][2]);

    aa0 = vec_left1_bin(b0,c0);
    aa1 = vec_left1_bin(b1,c1);

    cc0 = vec_right1_bin(c0,a0);
    cc1 = vec_right1_bin(c1,a1);

    and1_0 = vAND3(aa0, c0, cc0);
    and1_1 = vAND3(aa1, c1, cc1);

    and1 = vAND3(and1_0, and1_0, and1_1);
    and4 = and1;

    b0 = _mm_bitshift_left(a0, 127-nb_unused_col);
    b1 = _mm_bitshift_left(a1, 127-nb_unused_col);

    aa0 = vec_left1_bin(c0,a0);
    aa1 = vec_left1_bin(c1,a1);

    cc0 = vec_right1_bin_unused_col(a0, b0, nb_unused_col);
    cc1 = vec_right1_bin_unused_col(a1, b1, nb_unused_col);

    and2_0 = vAND3(aa0, a0, cc0);
    and2_1 = vAND3(aa1, a1, cc1);

    and2 = vAND3(and2_0, and2_0, and2_1);
    and5 = and2;

    aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
    cc0 = vec_right1_bin(and0,and1);
    or0 = vOR3(aa0, and0, cc0);

    aa0 = vec_left1_bin(and0,and1);
    cc0 = vec_right1_bin(and1,and2);
    or0_bis = vOR3(aa0, and1, cc0);

    aa0 = vec_left1_bin(and1,and2);
    cc0 = vec_right1_bin_unused_col(and2, _mm_bitshift_left(and2, 127-nb_unused_col),nb_unused_col);
    or0_ter = vOR3(aa0, and2, cc0);

    or1 = or0;
    or1_bis = or0_bis;
    or1_ter = or0_ter;

    for(int i = 0; i < height-3; i+=3){
      b2 = vec_load(&img_bin_extra_lines[i+2][0]);
      c2 = vec_load(&img_bin_extra_lines[i+2][1]);
      a2 = _mm_bitshift_right(b2, 127);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_2 = vAND3(aa2, b2, cc2);
      and6 = vAND3(and0_2, and0_1, and0_0);

      a2 = vec_load(&img_bin_extra_lines[i+2][2]);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_2 = vAND3(aa2, c2, cc2);
      and7 = vAND3(and1_2, and1_1, and1_0);

      b2 = _mm_bitshift_left(a2, 127-nb_unused_col);

      aa2 = vec_left1_bin(c2,a2);
      cc2 = vec_right1_bin_unused_col(a2,b2,nb_unused_col);

      and2_2 = vAND3(aa2, a2, cc2);
      and8 = vAND3(and2_2, and2_1, and2_0);

      aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
      cc0 = vec_right1_bin(and6,and7);
      or2 = vOR3(aa0, and6, cc0);

      aa0 = vec_left1_bin(and6,and7);
      cc0 = vec_right1_bin(and7,and8);
      or2_bis = vOR3(aa0, and7, cc0);

      aa0 = vec_left1_bin(and7,and8);
      cc0 = vec_right1_bin_unused_col(and8, _mm_bitshift_left(and8, 127-nb_unused_col),nb_unused_col);
      or2_ter = vOR3(aa0, and8, cc0);

      y = vOR3(or0, or1, or2);
      vec_store(&m[i][0], y);

      y = vOR3(or0_bis, or1_bis, or2_bis);
      vec_store(&m[i][1], y);

      y = vOR3(or0_ter, or1_ter, or2_ter);
      vec_store(&m[i][2], y);


      b2 = vec_load(&img_bin_extra_lines[i+3][0]);
      c2 = vec_load(&img_bin_extra_lines[i+3][1]);
      a2 = _mm_bitshift_right(b2, 127);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_0 = vAND3(aa2, b2, cc2);
      and0 = vAND3(and0_0, and0_2, and0_1);

      a2 = vec_load(&img_bin_extra_lines[i+3][2]);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_0 = vAND3(aa2, c2, cc2);
      and1 = vAND3(and1_0, and1_2, and1_1);

      b2 = _mm_bitshift_left(a2, 127-nb_unused_col);

      aa2 = vec_left1_bin(c2,a2);
      cc2 = vec_right1_bin_unused_col(a2,b2,nb_unused_col);

      and2_0 = vAND3(aa2, a2, cc2);
      and2 = vAND3(and2_0, and2_2, and2_1);

      aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
      cc0 = vec_right1_bin(and0,and1);
      or0 = vOR3(aa0, and0, cc0);

      aa0 = vec_left1_bin(and0,and1);
      cc0 = vec_right1_bin(and1,and2);
      or0_bis = vOR3(aa0, and1, cc0);

      aa0 = vec_left1_bin(and1,and2);
      cc0 = vec_right1_bin_unused_col(and2, _mm_bitshift_left(and2, 127-nb_unused_col),nb_unused_col);
      or0_ter = vOR3(aa0, and2, cc0);

      y = vOR3(or0, or2, or1);
      vec_store(&m[i+1][0], y);

      y = vOR3(or0_bis, or2_bis, or1_bis);
      vec_store(&m[i+1][1], y);

      y = vOR3(or0_ter, or2_ter, or1_ter);
      vec_store(&m[i+1][2], y);


      b2 = vec_load(&img_bin_extra_lines[i+4][0]);
      c2 = vec_load(&img_bin_extra_lines[i+4][1]);
      a2 = _mm_bitshift_right(b2, 127);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_1 = vAND3(aa2, b2, cc2);
      and3 = vAND3(and0_1, and0_0, and0_2);

      a2 = vec_load(&img_bin_extra_lines[i+4][2]);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_1 = vAND3(aa2, c2, cc2);
      and4 = vAND3(and1_1, and1_0, and1_2);

      b2 = _mm_bitshift_left(a2, 127-nb_unused_col);

      aa2 = vec_left1_bin(c2,a2);
      cc2 = vec_right1_bin_unused_col(a2,b2,nb_unused_col);

      and2_1 = vAND3(aa2, a2, cc2);
      and5 = vAND3(and2_1, and2_0, and2_2);

      aa0 = vec_left1_bin(_mm_bitshift_right(and3, 127),and3);
      cc0 = vec_right1_bin(and3,and4);
      or1 = vOR3(aa0, and3, cc0);

      aa0 = vec_left1_bin(and3,and4);
      cc0 = vec_right1_bin(and4,and5);
      or1_bis = vOR3(aa0, and4, cc0);

      aa0 = vec_left1_bin(and4,and5);
      cc0 = vec_right1_bin_unused_col(and5, _mm_bitshift_left(and5, 127-nb_unused_col),nb_unused_col);
      or1_ter = vOR3(aa0, and5, cc0);

      y = vOR3(or1, or0, or2);
      vec_store(&m[i+2][0], y);

      y = vOR3(or1_bis, or0_bis, or2_bis);
      vec_store(&m[i+2][1], y);

      y = vOR3(or1_ter, or0_ter, or2_ter);
      vec_store(&m[i+2][2], y);
    }
    switch (n) {
      case 2:
        b2 = vec_load(&img_bin_extra_lines[height][0]);
        c2 = vec_load(&img_bin_extra_lines[height][1]);
        a2 = _mm_bitshift_right(b2, 127);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_2 = vAND3(aa2, b2, cc2);
        and6 = vAND3(and0_2, and0_1, and0_0);

        a2 = vec_load(&img_bin_extra_lines[height][2]);

        aa2 = vec_left1_bin(b2,c2);
        cc2 = vec_right1_bin(c2,a2);

        and1_2 = vAND3(aa2, c2, cc2);
        and7 = vAND3(and1_2, and1_1, and1_0);

        b2 = _mm_bitshift_left(a2, 127-nb_unused_col);

        aa2 = vec_left1_bin(c2,a2);
        cc2 = vec_right1_bin_unused_col(a2,b2,nb_unused_col);

        and2_2 = vAND3(aa2, a2, cc2);
        and8 = vAND3(and2_2, and2_1, and2_0);

        aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
        cc0 = vec_right1_bin(and6,and7);
        or2 = vOR3(aa0, and6, cc0);

        aa0 = vec_left1_bin(and6,and7);
        cc0 = vec_right1_bin(and7,and8);
        or2_bis = vOR3(aa0, and7, cc0);

        aa0 = vec_left1_bin(and7,and8);
        cc0 = vec_right1_bin_unused_col(and8, _mm_bitshift_left(and8, 127-nb_unused_col),nb_unused_col);
        or2_ter = vOR3(aa0, and8, cc0);

        y = vOR3(or0, or1, or2);
        vec_store(&m[height-2][0], y);

        y = vOR3(or0_bis, or1_bis, or2_bis);
        vec_store(&m[height-2][1], y);

        y = vOR3(or0_ter, or1_ter, or2_ter);
        vec_store(&m[height-2][2], y);


        y = vOR3(or1, or2, or2);
        vec_store(&m[height-1][0], y);
        y = vOR3(or1_bis, or2_bis, or2_bis);
        vec_store(&m[height-1][1], y);
        y = vOR3(or1_ter, or2_ter, or2_ter);
        vec_store(&m[height-1][2], y);
      break;
      case 1 :
        y = vOR3(or0, or1, or1);
        vec_store(&m[height-1][0], y);
        y = vOR3(or0_bis, or1_bis, or1_bis);
        vec_store(&m[height-1][1], y);
        y = vOR3(or0_ter, or1_ter, or1_ter);
        vec_store(&m[height-1][2], y);
      break;
      case 0 :
        b2 = vec_load(&img_bin_extra_lines[height-1][0]);
        c2 = vec_load(&img_bin_extra_lines[height-1][1]);
        a2 = _mm_bitshift_right(b2, 127);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_2 = vAND3(aa2, b2, cc2);
        and6 = vAND3(and0_0, and0_2, and0_1);

        a2 = vec_load(&img_bin_extra_lines[height-1][2]);

        aa2 = vec_left1_bin(b2,c2);
        cc2 = vec_right1_bin(c2,a2);

        and1_2 = vAND3(aa2, c2, cc2);
        and7 = vAND3(and1_0, and1_2, and1_1);

        b2 = _mm_bitshift_left(a2, 127-nb_unused_col);

        aa2 = vec_left1_bin(c2,a2);
        cc2 = vec_right1_bin_unused_col(a2,b2,nb_unused_col);

        and2_2 = vAND3(aa2, a2, cc2);
        and8 = vAND3(and2_0, and2_2, and2_1);

        aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
        cc0 = vec_right1_bin(and6,and7);
        or2 = vOR3(aa0, and6, cc0);

        aa0 = vec_left1_bin(and6,and7);
        cc0 = vec_right1_bin(and7,and8);
        or2_bis = vOR3(aa0, and7, cc0);

        aa0 = vec_left1_bin(and7,and8);
        cc0 = vec_right1_bin_unused_col(and8, _mm_bitshift_left(and8, 127-nb_unused_col),nb_unused_col);
        or2_ter = vOR3(aa0, and8, cc0);

        y = vOR3(or0, or1, or2);
        vec_store(&m[height-3][0], y);

        y = vOR3(or0_bis, or1_bis, or2_bis);
        vec_store(&m[height-3][1], y);

        y = vOR3(or0_ter, or1_ter, or2_ter);
        vec_store(&m[height-3][2], y);


        b2 = vec_load(&img_bin_extra_lines[height][0]);
        c2 = vec_load(&img_bin_extra_lines[height][1]);
        a2 = _mm_bitshift_right(b2, 127);

        aa2 = vec_left1_bin(a2,b2);
        cc2 = vec_right1_bin(b2,c2);

        and0_0 = vAND3(aa2, b2, cc2);
        and0 = vAND3(and0_1, and0_2, and0_0);

        a2 = vec_load(&img_bin_extra_lines[height][2]);

        aa2 = vec_left1_bin(b2,c2);
        cc2 = vec_right1_bin(c2,a2);

        and1_0 = vAND3(aa2, c2, cc2);
        and1 = vAND3(and1_1, and1_0, and1_2);

        b2 = _mm_bitshift_left(a2, 127-nb_unused_col);

        aa2 = vec_left1_bin(c2,a2);
        cc2 = vec_right1_bin_unused_col(a2,b2,nb_unused_col);

        and2_0 = vAND3(aa2, a2, cc2);
        and2 = vAND3(and2_1, and2_2, and2_0);

        aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
        cc0 = vec_right1_bin(and0,and1);
        or0 = vOR3(aa0, and0, cc0);

        aa0 = vec_left1_bin(and0,and1);
        cc0 = vec_right1_bin(and1,and2);
        or0_bis = vOR3(aa0, and1, cc0);

        aa0 = vec_left1_bin(and1,and2);
        cc0 = vec_right1_bin_unused_col(and2, _mm_bitshift_left(and2, 127-nb_unused_col),nb_unused_col);
        or0_ter = vOR3(aa0, and2, cc0);

        y = vOR3(or0, or2, or1);
        vec_store(&m[height-2][0], y);

        y = vOR3(or0_bis, or2_bis, or1_bis);
        vec_store(&m[height-2][1], y);

        y = vOR3(or0_ter, or2_ter, or1_ter);
        vec_store(&m[height-2][2], y);


        y = vOR3(or2, or0, or0);
        vec_store(&m[height-1][0], y);
        y = vOR3(or2_bis, or0_bis, or0_bis);
        vec_store(&m[height-1][1], y);
        y = vOR3(or2_ter, or0_ter, or0_ter);
        vec_store(&m[height-1][2], y);
      break;
    }

    _mm_free(img_bin_extra_lines-1);
    return m;
  }






  // CAS DES IMAGES DE 4*128 COLONNES OU PLUS
  // Première et deuxième colonne
  b0 = vec_load(&img_bin_extra_lines[-1][0]);
  b1 = vec_load(&img_bin_extra_lines[-0][0]);
  b2 = vec_load(&img_bin_extra_lines[1][0]);

  c0 = vec_load(&img_bin_extra_lines[-1][1]);
  c1 = vec_load(&img_bin_extra_lines[-0][1]);
  c2 = vec_load(&img_bin_extra_lines[1][1]);

  a0 = _mm_bitshift_right(b0, 127);
  a1 = _mm_bitshift_right(b1, 127);
  a2 = _mm_bitshift_right(b2, 127);

  aa0 = vec_left1_bin(a0,b0);
  aa1 = vec_left1_bin(a1,b1);
  aa2 = vec_left1_bin(a2,b2);

  cc0 = vec_right1_bin(b0,c0);
  cc1 = vec_right1_bin(b1,c1);
  cc2 = vec_right1_bin(b2,c2);

  and0_0 = vAND3(aa0, b0, cc0);
  and0_1 = vAND3(aa1, b1, cc1);
  and0_2 = vAND3(aa2, b2, cc2);

  and0 = vAND3(and0_0, and0_0, and0_1);
  and3 = vAND3(and0_0, and0_1, and0_2);

  a0 = vec_load(&img_bin_extra_lines[-1][2]);
  a1 = vec_load(&img_bin_extra_lines[-0][2]);
  a2 = vec_load(&img_bin_extra_lines[1][2]);

  aa0 = vec_left1_bin(b0,c0);
  aa1 = vec_left1_bin(b1,c1);
  aa2 = vec_left1_bin(b2,c2);

  cc0 = vec_right1_bin(c0,a0);
  cc1 = vec_right1_bin(c1,a1);
  cc2 = vec_right1_bin(c2,a2);

  and1_0 = vAND3(aa0, c0, cc0);
  and1_1 = vAND3(aa1, c1, cc1);
  and1_2 = vAND3(aa2, c2, cc2);

  and1 = vAND3(and1_0, and1_0, and1_1);
  and4 = vAND3(and1_0, and1_1, and1_2);

  b0 = vec_load(&img_bin_extra_lines[-1][3]);
  b1 = vec_load(&img_bin_extra_lines[-0][3]);
  b2 = vec_load(&img_bin_extra_lines[1][3]);

  aa0 = vec_left1_bin(c0,a0);
  aa1 = vec_left1_bin(c1,a1);
  aa2 = vec_left1_bin(c2,a2);

  cc0 = vec_right1_bin(a0,b0);
  cc1 = vec_right1_bin(a1,b1);
  cc2 = vec_right1_bin(a2,b2);

  and2_0 = vAND3(aa0, a0, cc0);
  and2_1 = vAND3(aa1, a1, cc1);
  and2_2 = vAND3(aa2, a2, cc2);

  and2 = vAND3(and2_0, and2_0, and2_1);
  and5 = vAND3(and2_0, and2_1, and2_2);

  aa0 = vec_left1_bin(_mm_bitshift_right(and0, 127),and0);
  cc0 = vec_right1_bin(and0,and1);
  or0 = vOR3(aa0, and0, cc0);

  aa0 = vec_left1_bin(and0,and1);
  cc0 = vec_right1_bin(and1,and2);
  or0_bis = vOR3(aa0, and1, cc0);

  aa0 = vec_left1_bin(_mm_bitshift_right(and3, 127),and3);
  cc0 = vec_right1_bin(and3,and4);
  or1 = vOR3(aa0, and3, cc0);

  aa0 = vec_left1_bin(and3,and4);
  cc0 = vec_right1_bin(and4,and5);
  or1_bis = vOR3(aa0, and4, cc0);

  for(int i = 0; i < height-1; i++){
    b2 = vec_load(&img_bin_extra_lines[i+2][0]);
    c2 = vec_load(&img_bin_extra_lines[i+2][1]);
    a2 = _mm_bitshift_right(b2, 127);

    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);

    and0_0 = vAND3(aa2, b2, cc2);
    and6 = vAND3(and0_1, and0_2, and0_0);

    a2 = vec_load(&img_bin_extra_lines[i+2][2]);

    aa2 = vec_left1_bin(b2,c2);
    cc2 = vec_right1_bin(c2,a2);

    and1_0 = vAND3(aa2, c2, cc2);
    and7 = vAND3(and1_1, and1_2, and1_0);

    b2 = vec_load(&img_bin_extra_lines[i+2][3]);

    aa2 = vec_left1_bin(c2,a2);
    cc2 = vec_right1_bin(a2,b2);

    and2_0 = vAND3(aa2, a2, cc2);
    and8 = vAND3(and2_1, and2_2, and2_0);

    aa0 = vec_left1_bin(_mm_bitshift_right(and6, 127),and6);
    cc0 = vec_right1_bin(and6,and7);
    or2 = vOR3(aa0, and6, cc0);

    aa0 = vec_left1_bin(and6,and7);
    cc0 = vec_right1_bin(and7,and8);
    or2_bis = vOR3(aa0, and7, cc0);

    y = vOR3(or0, or1, or2);
    vec_store(&m[i][0], y);
    y = vOR3(or0_bis, or1_bis, or2_bis);
    vec_store(&m[i][1], y);

    and0_1 = and0_2; and0_2 = and0_0;
    and1_1 = and1_2; and1_2 = and1_0;
    and2_1 = and2_2; and2_2 = and2_0;
    and0 = and3; and1 = and4; and2 = and5;
    and3 = and6; and4 = and7; and5 = and8;
    or0 = or1; or1 = or2;
    or0_bis = or1_bis; or1_bis = or2_bis;
  }
  y = vOR3(or1, or2, or2);
  vec_store(&m[height-1][0], y);
  y = vOR3(or1_bis, or2_bis, or2_bis);
  vec_store(&m[height-1][1], y);

  for(int j = 2; j < nb_vbits_col-2; j++){
    a0 = vec_load(&img_bin_extra_lines[-1][j-2]);
    a1 = vec_load(&img_bin_extra_lines[-0][j-2]);
    a2 = vec_load(&img_bin_extra_lines[1][j-2]);

    b0 = vec_load(&img_bin_extra_lines[-1][j-1]);
    b1 = vec_load(&img_bin_extra_lines[-0][j-1]);
    b2 = vec_load(&img_bin_extra_lines[1][j-1]);

    c0 = vec_load(&img_bin_extra_lines[-1][j-0]);
    c1 = vec_load(&img_bin_extra_lines[-0][j-0]);
    c2 = vec_load(&img_bin_extra_lines[1][j-0]);

    aa0 = vec_left1_bin(a0,b0);
    aa1 = vec_left1_bin(a1,b1);
    aa2 = vec_left1_bin(a2,b2);

    cc0 = vec_right1_bin(b0,c0);
    cc1 = vec_right1_bin(b1,c1);
    cc2 = vec_right1_bin(b2,c2);

    and0_0 = vAND3(aa0, b0, cc0);
    and0_1 = vAND3(aa1, b1, cc1);
    and0_2 = vAND3(aa2, b2, cc2);

    and0 = vAND3(and0_0, and0_0, and0_1);
    and3 = vAND3(and0_0, and0_1, and0_2);

    a0 = vec_load(&img_bin_extra_lines[-1][j+1]);
    a1 = vec_load(&img_bin_extra_lines[-0][j+1]);
    a2 = vec_load(&img_bin_extra_lines[1][j+1]);

    aa0 = vec_left1_bin(b0,c0);
    aa1 = vec_left1_bin(b1,c1);
    aa2 = vec_left1_bin(b2,c2);

    cc0 = vec_right1_bin(c0,a0);
    cc1 = vec_right1_bin(c1,a1);
    cc2 = vec_right1_bin(c2,a2);

    and1_0 = vAND3(aa0, c0, cc0);
    and1_1 = vAND3(aa1, c1, cc1);
    and1_2 = vAND3(aa2, c2, cc2);

    and1 = vAND3(and1_0, and1_0, and1_1);
    and4 = vAND3(and1_0, and1_1, and1_2);

    b0 = vec_load(&img_bin_extra_lines[-1][j+2]);
    b1 = vec_load(&img_bin_extra_lines[-0][j+2]);
    b2 = vec_load(&img_bin_extra_lines[1][j+2]);

    aa0 = vec_left1_bin(c0,a0);
    aa1 = vec_left1_bin(c1,a1);
    aa2 = vec_left1_bin(c2,a2);

    cc0 = vec_right1_bin(a0,b0);
    cc1 = vec_right1_bin(a1,b1);
    cc2 = vec_right1_bin(a2,b2);

    and2_0 = vAND3(aa0, a0, cc0);
    and2_1 = vAND3(aa1, a1, cc1);
    and2_2 = vAND3(aa2, a2, cc2);

    and2 = vAND3(and2_0, and2_0, and2_1);
    and5 = vAND3(and2_0, and2_1, and2_2);

    aa0 = vec_left1_bin(and0, and1);
    cc0 = vec_right1_bin(and1, and2);
    or0 = vOR3(aa0, and1, cc0);

    aa1 = vec_left1_bin(and3, and4);
    cc1 = vec_right1_bin(and4, and5);
    or1 = vOR3(aa1, and4, cc1);

    for(int i = 0; i < height-1; i++){
      a2 = vec_load(&img_bin_extra_lines[i+2][j-2]);
      b2 = vec_load(&img_bin_extra_lines[i+2][j-1]);
      c2 = vec_load(&img_bin_extra_lines[i+2][j-0]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      and0_0 = vAND3(aa2, b2, cc2);
      and6 = vAND3(and0_1, and0_2, and0_0);

      a2 = vec_load(&img_bin_extra_lines[i+2][j+1]);

      aa2 = vec_left1_bin(b2,c2);
      cc2 = vec_right1_bin(c2,a2);

      and1_0 = vAND3(aa2, c2, cc2);
      and7 = vAND3(and1_1, and1_2, and1_0);

      b2 = vec_load(&img_bin_extra_lines[i+2][j+2]);

      aa2 = vec_left1_bin(c2,a2);
      cc2 = vec_right1_bin(a2,b2);

      and2_0 = vAND3(aa2, a2, cc2);
      and8 = vAND3(and2_1, and2_2, and2_0);

      aa2 = vec_left1_bin(and6, and7);
      cc2 = vec_right1_bin(and7, and8);
      or2 = vOR3(aa2, and7, cc2);

      y = vOR3(or0, or1, or2);
      vec_store(&m[i][j], y);

      or0 = or1; or1 = or2;
      and0 = and3; and1 = and4; and2 = and5;
      and3 = and6; and4 = and7; and5 = and8;
      and0_1 = and0_2; and0_2 = and0_0;
      and1_1 = and1_2; and1_2 = and1_0;
      and2_1 = and2_2; and2_2 = and2_0;
    }
    y = vOR3(or1, or2, or2);
    vec_store(&m[height-1][j], y);
  }


  // Avant dernière et dernière colonne
  b0 = vec_load(&img_bin_extra_lines[-1][nb_vbits_col-1]);
  b1 = vec_load(&img_bin_extra_lines[-0][nb_vbits_col-1]);
  b2 = vec_load(&img_bin_extra_lines[1][nb_vbits_col-1]);

  a0 = vec_load(&img_bin_extra_lines[-1][nb_vbits_col-2]);
  a1 = vec_load(&img_bin_extra_lines[-0][nb_vbits_col-2]);
  a2 = vec_load(&img_bin_extra_lines[1][nb_vbits_col-2]);

  c0 = _mm_bitshift_left(b0, 127-nb_unused_col);
  c1 = _mm_bitshift_left(b1, 127-nb_unused_col);
  c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

  aa0 = vec_left1_bin(a0,b0);
  aa1 = vec_left1_bin(a1,b1);
  aa2 = vec_left1_bin(a2,b2);

  cc0 = vec_right1_bin_unused_col(b0,c0,nb_unused_col);
  cc1 = vec_right1_bin_unused_col(b1,c1,nb_unused_col);
  cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);

  and0_0 = vAND3(aa0, b0, cc0);
  and0_1 = vAND3(aa1, b1, cc1);
  and0_2 = vAND3(aa2, b2, cc2);

  and0 = vAND3(and0_0, and0_0, and0_1);
  and3 = vAND3(and0_0, and0_1, and0_2);

  c0 = vec_load(&img_bin_extra_lines[-1][nb_vbits_col-3]);
  c1 = vec_load(&img_bin_extra_lines[-0][nb_vbits_col-3]);
  c2 = vec_load(&img_bin_extra_lines[1][nb_vbits_col-3]);

  aa0 = vec_left1_bin(c0,a0);
  aa1 = vec_left1_bin(c1,a1);
  aa2 = vec_left1_bin(c2,a2);

  cc0 = vec_right1_bin(a0,b0);
  cc1 = vec_right1_bin(a1,b1);
  cc2 = vec_right1_bin(a2,b2);

  and1_0 = vAND3(aa0, a0, cc0);
  and1_1 = vAND3(aa1, a1, cc1);
  and1_2 = vAND3(aa2, a2, cc2);

  and1 = vAND3(and1_0, and1_0, and1_1);
  and4 = vAND3(and1_0, and1_1, and1_2);

  b0 = vec_load(&img_bin_extra_lines[-1][nb_vbits_col-4]);
  b1 = vec_load(&img_bin_extra_lines[-0][nb_vbits_col-4]);
  b2 = vec_load(&img_bin_extra_lines[1][nb_vbits_col-3]);

  aa0 = vec_left1_bin(b0,c0);
  aa1 = vec_left1_bin(b1,c1);
  aa2 = vec_left1_bin(b2,c2);

  cc0 = vec_right1_bin(c0,a0);
  cc1 = vec_right1_bin(c1,a1);
  cc2 = vec_right1_bin(c2,a2);

  and2_0 = vAND3(aa0, c0, cc0);
  and2_1 = vAND3(aa1, c1, cc1);
  and2_2 = vAND3(aa2, c2, cc2);

  and2 = vAND3(and2_0, and2_0, and2_1);
  and5 = vAND3(and2_0, and2_1, and2_2);

  aa0 = vec_left1_bin(and1,and0);
  cc0 = vec_right1_bin_unused_col(and0, _mm_bitshift_left(and0, 127-nb_unused_col),nb_unused_col);
  or0 = vOR3(aa0, and0, cc0);

  aa0 = vec_left1_bin(and2,and1);
  cc0 = vec_right1_bin(and1,and0);
  or0_bis = vOR3(aa0, and1, cc0);

  aa0 = vec_left1_bin(and4,and3);
  cc0 = vec_right1_bin_unused_col(and3, _mm_bitshift_left(and3, 127-nb_unused_col),nb_unused_col);
  or1 = vOR3(aa0, and3, cc0);

  aa0 = vec_left1_bin(and5,and4);
  cc0 = vec_right1_bin(and4,and3);
  or1_bis = vOR3(aa0, and4, cc0);

  for(int i = 0; i < height-1; i++){
    a2 = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-2]);
    b2 = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-1]);
    c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);

    and0_0 = vAND3(aa2, b2, cc2);
    and6 = vAND3(and0_1, and0_2, and0_0);

    c2 = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-3]);

    aa2 = vec_left1_bin(c2,a2);
    cc2 = vec_right1_bin(a2,b2);

    and1_0 = vAND3(aa2, a2, cc2);
    and7 = vAND3(and1_2, and1_1, and1_0);

    b2 = vec_load(&img_bin_extra_lines[i+2][nb_vbits_col-4]);

    aa2 = vec_left1_bin(b2,c2);
    cc2 = vec_right1_bin(c2,a2);

    and2_0 = vAND3(aa2, c2, cc2);
    and8 = vAND3(and2_2, and2_1, and2_0);

    aa0 = vec_left1_bin(and7,and6);
    cc0 = vec_right1_bin_unused_col(and6, _mm_bitshift_left(and6, 127-nb_unused_col),nb_unused_col);
    or2 = vOR3(aa0, and6, cc0);

    aa0 = vec_left1_bin(and8,and7);
    cc0 = vec_right1_bin(and7,and6);
    or2_bis = vOR3(aa0, and7, cc0);

    y = vOR3(or0, or1, or2);
    vec_store(&m[i][nb_vbits_col-1], y);
    y = vOR3(or0_bis, or1_bis, or2_bis);
    vec_store(&m[i][nb_vbits_col-2], y);

    and0_1 = and0_2; and0_2 = and0_0;
    and1_1 = and1_2; and1_2 = and1_0;
    and2_1 = and2_2; and2_2 = and2_0;
    and0 = and3; and1 = and4; and2 = and5;
    and3 = and6; and4 = and7; and5 = and8;
    or0 = or1; or1 = or2;
    or0_bis = or1_bis; or1_bis = or2_bis;
  }
  y = vOR3(or1, or2, or2);
  vec_store(&m[height-1][nb_vbits_col-1], y);
  y = vOR3(or1_bis, or2_bis, or2_bis);
  vec_store(&m[height-1][nb_vbits_col-2], y);

  _mm_free(img_bin_extra_lines-1);
  return m;
}
