#pragma once

typedef struct {
  int dimension;
  int divisor;
  int *data;
} Kernel;

Kernel *create_mean_3x3();
Kernel *create_mean_5x5();
Kernel *create_mean_7x7();
Kernel *create_horizontal_schar();
Kernel *create_vertical_schar();
Kernel *create_laplacian_20();
Kernel *create_laplacian_4();

int write_kernel_to_file(const Kernel *kernel, const char *filename);
Kernel *read_kernel_from_file(const char *filename);
void free_kernel(Kernel *kernel);
void print_kernel(const Kernel *kernel);
