#include "kernel_io.h"

unsigned char **apply_filter(unsigned char **input, int rows, int cols,
                             Kernel *(k)());

unsigned char **apply_sobel(unsigned char **input, int rows, int cols,
                             Kernel *(kx)(), Kernel *(ky)());

unsigned char **gradient_filter(unsigned char **input, int rows, int cols);
double* gradient(double (*f)(double *, int), double *p, const int dim);
double l1_norm(double *f, int dim);
