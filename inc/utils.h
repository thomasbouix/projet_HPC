// Error if a is true
#define CHECK_ERROR(a)                        \
if (a) {                                      \
	fprintf(stderr, "*** Error :\n" #a "\n"); \
	exit(1);                                  \
}

#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)
