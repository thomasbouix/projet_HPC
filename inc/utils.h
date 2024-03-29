#ifndef UTILS_H
#define UTILS_H

	// #define BENCH

	#if defined(__INTEL_COMPILER) || defined(__ICC)
	#include "ia32intrin.h" // chrono Intel
	#else

	#ifdef __GNUC__
	// car ICC definie aussi GNUC dans certains cas (?)
	#include "x86intrin.h" // chrono GCC
	#endif

	#endif

	#include <time.h>
	#include <math.h>
	#include <stdint.h>
	#include <omp.h>
	#include <math.h>
	#include <assert.h>
	#include "vnrdef.h"
	#include "simd_macro.h"

	#define ERROR(message) 															\
		printf("\n");																			\
		fprintf(stderr, "*** \033[1;31mError\033[0m : ");	\
		fprintf(stderr, message);													\
		fprintf(stderr, "\n");														\
		exit(-1);

	#define SUCCESS(message)									\
		printf("\033[1;32mSucess\033[0m : ");		\
	 	printf(message);												\
		printf("\n");														\

	// Error if a is true
	#define CHECK_ERROR(a)                        \
	if (a) {                                      \
		fprintf(stderr, "*** Error :\n" #a "\n"); \
		exit(1);                                  \
	}

	#define CHRONO(X,t)                       \
	    tmin = 1e38;                          \
	    for(run=0; run<nrun; run++) {         \
	        t0 = (double) _rdtsc();           \
	        for(iter=0; iter<niter; iter++) { \
	            X;                            \
	        }                                 \
	        t1 = (double) _rdtsc();           \
	        dt=t1-t0; if(dt<tmin) tmin = dt;  \
	    }                                     \
	    t = tmin / (double) niter

	#define MAX(a,b) ((a >= b) ? a : b)
	#define MIN(a,b) ((a <= b) ? a : b)

	typedef __m128i vbits;

  void convert_coding(uint8** img, int nrl, int nrh, int ncl, int nch, int from, int to);
  uint8** LoadPGM_padding_ui8matrix(char *filename, int* nrl, int* nrh, int* ncl, int* nch, int padding);
  void free_padding_ui8matrix(uint8** img_with_padding, int nrl, int nrh, int ncl, int nch, int padding);
  vuint8** LoadPGM_vui8matrix(char *filename, int* si0, int* si1, int* sj0, int* sj1, int* vi0, int* vi1, int* vj0, int* vj1);
  uint8** add_borders(uint8 **m, int height, int width, int border);
	void zero_ui8matrix(uint8 *** m, int nrl, int nrh, int ncl, int nch);
	void set_ui8matrix(uint8 *** m, int nrl, int nrh, int ncl, int nch, uint8 value);
	// value is 0 or 1
	void set_ui8matrix_0255(uint8 *** m, int nrl, int nrh, int ncl, int nch);
	// remplie les bordures d'une matrice avec une valeur
	void set_ui8_bordures(uint8 *** m, int nrl, int nrh, int ncl, int nch, uint8 val);
	vbits** convert_to_binary(uint8** img, size_t height, size_t width);
	uint8** convert_from_binary(vbits** binary_img, int height, int width);
	void free_vbitsmatrix(vbits **m, int height, int width);
	void display_hexa_vbits_matrix(vbits** m, int height, int width);
	void display_hexa_vbits(vbits v);

	// Méthode inspirée de Stack overflow (voir schema explicatif dans le rapport)
	__m128i _mm_bitshift_left(__m128i x, unsigned char offset);
	__m128i _mm_bitshift_right(__m128i x, unsigned char offset);

	// m[i][j] bitwise
	int get_bit(vbits** m, int i, int j);
	// m[i][j] = value, avec value=0 ou value=1
	void set_bit(vbits **m, int i, int j, int value);

	// compare deux matrices SIMD, renvoie 1 si égales, 0 sinon
	int compare_SIMD(vbits ** m0, vbits ** m1, int height, int width);
#endif
