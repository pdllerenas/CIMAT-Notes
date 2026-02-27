#include "kernel_io.h"
#include "compare.h"

unsigned char ** apply_mean(unsigned char **, int, int, Kernel*(k)(int), int, int);
unsigned char **median_filter_nxn(unsigned char **, int, int, int, int);
