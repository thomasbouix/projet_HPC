#include "morpho_SIMD.h"

// AND logique sur un voisinage de taille 3x3
vbits** erosion_3x3_SIMD(vbits** img_bin, int height, int width)
{
  int nb_vbits_col = ceil((float)width/128);
  int nb_unused_col = (128-(width%128))%128;
  vbits **m = (vbits**)vui32matrix(0, height-1, 0, nb_vbits_col-1);

  // Ajout d'un pointeur vers les lignes 0 et height-1 de l'image pour gérer plus facilement les bords
  vbits **img_bin_extra_lines=(vbits **) _mm_malloc ((size_t)((height+2)*sizeof(vbits*)), 16);
  if (!img_bin_extra_lines) vnrerror("allocation failure 1 in erosion_3x3()");
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

  // Cas particulier des images à moins de 128 colonnes
  if(nb_vbits_col == 1){

    // PROLOGUE
    int n = height%3;

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
  for(int j = 1; j < nb_vbits_col-1; j++){
    // PRELOAD
    a0 = vec_load(&img_bin_extra_lines[-1][j-1]);
    b0 = vec_load(&img_bin_extra_lines[-1][j+0]);
    c0 = vec_load(&img_bin_extra_lines[-1][j+1]);

    a1 = vec_load(&img_bin_extra_lines[0][j-1]);
    b1 = vec_load(&img_bin_extra_lines[0][j+0]);
    c1 = vec_load(&img_bin_extra_lines[0][j+1]);

    aa0 = vec_left1_bin(a0, b0);
    cc0 = vec_right1_bin(b0, c0);
    aa1 = vec_left1_bin(a1, b1);
    cc1 = vec_right1_bin(b1, c1);

    and0 = vAND3(aa0, b0, cc0);
    and1 = vAND3(aa1, b1, cc1);

    for(int i = 0; i < height; i++){
      a2 = vec_load(&img_bin_extra_lines[i+1][j-1]);
      b2 = vec_load(&img_bin_extra_lines[i+1][j+0]);
      c2 = vec_load(&img_bin_extra_lines[i+1][j+1]);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);
      and2 = vAND3(aa2, b2, cc2);

      y = vAND3(and0, and1, and2);
      vec_store(&m[i][j], y);

      // ROTATION
      and0 = and1; and1 = and2;
    }
  }

  // Gestion des bords (première et dernière colonnes)
  // PRELOAD FIRST COL
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

  // PRELOAD LAST COL
  b0_last = vec_load(&img_bin_extra_lines[-1][nb_vbits_col-1]);
  a0_last = vec_load(&img_bin_extra_lines[-1][nb_vbits_col-2]);
  c0_last = _mm_bitshift_left(b0_last, 127-nb_unused_col);

  b1_last = vec_load(&img_bin_extra_lines[0][nb_vbits_col-1]);
  a1_last = vec_load(&img_bin_extra_lines[0][nb_vbits_col-2]);
  c1_last = _mm_bitshift_left(b1_last, 127-nb_unused_col);

  aa0_last = vec_left1_bin(a0, b0);
  cc0_last = vec_right1_bin_unused_col(b0, c0, nb_unused_col);
  aa1_last = vec_left1_bin(a1, b1);
  cc1_last = vec_right1_bin_unused_col(b1, c1, nb_unused_col);

  and0_last = vAND3(aa0, b0, cc0);
  and1_last = vAND3(aa1, b1, cc1);

  for(int i = 0; i < height; i++){
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

    // ROTATION
    and0 = and1; and1 = and2;
    and0_last = and1_last; and1_last = and2_last;
  }





  /*for(int i = 0; i < height; i++){
    // Gestion de la première colonne de la ligne
    b0 = vec_load(&img_bin_extra_lines[i-1][0]);
    c0 = vec_load(&img_bin_extra_lines[i-1][1]);
    a0 = _mm_bitshift_right(b0, 127);

    b1 = vec_load(&img_bin_extra_lines[i-0][0]);
    c1 = vec_load(&img_bin_extra_lines[i-0][1]);
    a1 = _mm_bitshift_right(b1, 127);

    b2 = vec_load(&img_bin_extra_lines[i+1][0]);
    c2 = vec_load(&img_bin_extra_lines[i+1][1]);
    a2 = _mm_bitshift_right(b2, 127);

    aa0 = vec_left1_bin(a0,b0);
    cc0 = vec_right1_bin(b0,c0);

    aa1 = vec_left1_bin(a1,b1);
    cc1 = vec_right1_bin(b1,c1);

    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin(b2,c2);

    y = vAND_2D_9(aa0, b0, cc0, aa1, b1, cc1, aa2, b2, cc2);
    vec_store(&m[i][0], y);

    a0 = b0; b0 = c0;
    a1 = b1; b1 = c1;
    a2 = b2; b2 = c2;

    for(int j = 1; j < nb_vbits_col-1; j++){

      c0 = vec_load(&img_bin_extra_lines[i-1][j+1]);
      c1 = vec_load(&img_bin_extra_lines[i-0][j+1]);
      c2 = vec_load(&img_bin_extra_lines[i+1][j+1]);

      aa0 = vec_left1_bin(a0,b0);
      cc0 = vec_right1_bin(b0,c0);

      aa1 = vec_left1_bin(a1,b1);
      cc1 = vec_right1_bin(b1,c1);

      aa2 = vec_left1_bin(a2,b2);
      cc2 = vec_right1_bin(b2,c2);

      y = vAND_2D_9(aa0, b0, cc0, aa1, b1, cc1, aa2, b2, cc2);
      vec_store(&m[i][j], y);

      a0 = b0; b0 = c0;
      a1 = b1; b1 = c1;
      a2 = b2; b2 = c2;
    }
    // Gestion de la dernière colonne de la ligne
    a0 = b0; a1 = b1; a2 = b2;
    b0 = c0; b1 = c1; b2 = c2;

    c0 = _mm_bitshift_left(b0, 127-nb_unused_col);
    c1 = _mm_bitshift_left(b1, 127-nb_unused_col);
    c2 = _mm_bitshift_left(b2, 127-nb_unused_col);

    aa0 = vec_left1_bin(a0,b0);
    cc0 = vec_right1_bin_unused_col(b0,c0,nb_unused_col);

    aa1 = vec_left1_bin(a1,b1);
    cc1 = vec_right1_bin_unused_col(b1,c1,nb_unused_col);

    aa2 = vec_left1_bin(a2,b2);
    cc2 = vec_right1_bin_unused_col(b2,c2,nb_unused_col);

    y = vAND_2D_9(aa0, b0, cc0, aa1, b1, cc1, aa2, b2, cc2);
    vec_store(&m[i][nb_vbits_col-1], y);
  }*/

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
