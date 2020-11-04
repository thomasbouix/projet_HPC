#ifndef UTILS_H
#define UTILS_H

// Error if a is true
#define CHECK_ERROR(a)                        \
if (a) {                                      \
	fprintf(stderr, "*** Error :\n" #a "\n"); \
	exit(1);                                  \
}

#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)

void convert_coding(uint8** img, int nrl, int nrh, int ncl, int nch, int from, int to);

#endif
