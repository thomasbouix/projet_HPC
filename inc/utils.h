#ifndef UTILS_H
#define UTILS_H

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
	#include "vnrdef.h"
	#include "simd_macro.h"

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
	vbits** convert_to_binary(uint8** img, size_t height, size_t width);
	uint8** convert_from_binary(vbits** binary_img, int height, int width);
	void free_vbitsmatrix(vbits **m, int height, int width);
	void display_hexa_vbits_matrix(vbits** m, int height, int width);
	void display_hexa_vbits(vbits v);

	// Méthode inspirée de Stack overflow (voir schema explicatif dans le rapport)
	__m128i _mm_bitshift_left(__m128i x, unsigned char offset);
	__m128i _mm_bitshift_right(__m128i x, unsigned char offset);

#endif
