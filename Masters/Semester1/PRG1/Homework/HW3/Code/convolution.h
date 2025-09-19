#include "kernel_io.h"

unsigned char **apply_gaussian(unsigned char **, int, int, Kernel *(k)());

unsigned char **apply_sobel(unsigned char **, int, int, Kernel *(kx)(),
                            Kernel *(ky)());
unsigned char ** apply_mean(unsigned char **, int, int, Kernel*(k)(int), int);

unsigned char **apply_gradient(unsigned char **, int, int);
unsigned char **median_filter_nxn(unsigned char **, int, int, int);
