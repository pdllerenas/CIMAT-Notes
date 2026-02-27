#pragma once

typedef struct {
  int dimension;
  int divisor;
  int *data;
} Kernel;

Kernel *create_mean_nxn(int);

int write_kernel_to_file(const Kernel *, const char *);
Kernel *read_kernel_from_file(const char *);
void free_kernel(Kernel *);
void print_kernel(const Kernel *);
