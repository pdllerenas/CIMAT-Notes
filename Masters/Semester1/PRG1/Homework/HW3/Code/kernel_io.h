#pragma once

typedef struct {
  int dimension;
  int divisor;
  int *data;
} Kernel;

Kernel *create_mean_nxn(int);
Kernel *create_gaussian_3x3();
Kernel *create_gaussian_5x5();
Kernel *create_gaussian_7x7();
Kernel *create_horizontal_schar();
Kernel *create_vertical_schar();
Kernel *create_laplacian_20();
Kernel *create_laplacian_4();

int write_kernel_to_file(const Kernel *, const char *);
Kernel *read_kernel_from_file(const char *);
void free_kernel(Kernel *);
void print_kernel(const Kernel *);
