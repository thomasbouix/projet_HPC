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

	#include "vnrdef.h"

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

  void convert_coding(uint8** img, int nrl, int nrh, int ncl, int nch, int from, int to);
  uint8** LoadPGM_padding_ui8matrix(char *filename, int* nrl, int* nrh, int* ncl, int* nch, int padding);
  void free_padding_ui8matrix(uint8** img_with_padding, int nrl, int nrh, int ncl, int nch, int padding);
  vuint8** LoadPGM_vui8matrix(char *filename, int* si0, int* si1, int* sj0, int* sj1, int* vi0, int* vi1, int* vj0, int* vj1);
  uint8** add_borders(uint8 **m, int height, int width, int border);
	void zero_ui8matrix(uint8 *** m, int nrl, int nrh, int ncl, int nch);
#endif
