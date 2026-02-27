#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel_io.h"

/*

the mean kernel aims to be used to calculate the mean of an nxn
subarray of an image, so this creates a kernel object consisting of 1's,
and the divisor is its size.

 */

Kernel *create_mean_nxn(int dim) {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = dim;
  k->divisor = dim * dim;
  k->data = malloc(dim * dim * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }
  for (int i = 0; i < dim * dim; i++) {
    k->data[i] = 1;
  }
  // store kernel in file
  // char filename[100];
  // sprintf(filename, "mean_%dx%d.bin", dim, dim);

  // write_kernel_to_file(k, filename);
  return k;
}


int write_kernel_to_file(const Kernel *kernel, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Could not open file for writing.\n");
    return -1;
  }
  fwrite(&kernel->dimension, sizeof(int), 1, file);
  fwrite(&kernel->divisor, sizeof(int), 1, file);
  fwrite(kernel->data, sizeof(int), kernel->dimension * kernel->dimension,
         file);
  fclose(file);
  return 0;
}

/*

reads a kernel stored as a .bin file. Returns a Kernel pointer

 */

Kernel *read_kernel_from_file(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    return NULL;
  }
  Kernel *k = malloc(sizeof(Kernel));

  if (!k) {
    fclose(file);
    return NULL;
  }

  if (fread(&k->dimension, sizeof(int), 1, file) != 1 ||
      fread(&k->divisor, sizeof(int), 1, file) != 1) {
    fprintf(stderr, "Error: Could not read kernel metadata from %s\n",
            filename);
    free(k);
    fclose(file);
    return NULL;
  }

  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    fclose(file);
    return NULL;
  }

  if (fread(k->data, sizeof(int), k->dimension * k->dimension, file) !=
      (size_t)(k->dimension * k->dimension)) {
    fprintf(stderr, "Error: Could not read kernel data from %s\n", filename);
    free_kernel(k);
    fclose(file);
    return NULL;
  }
  fclose(file);
  return k;
}

/*

prints the kernel attributes

 */

void print_kernel(const Kernel *kernel) {
  if (!kernel) {
    return;
  }

  for (int i = 0; i < kernel->dimension; i++) {
    for (int j = 0; j < kernel->dimension; j++) {
      printf("%2d ", kernel->data[i * kernel->dimension + j]);
    }
    printf("\n");
  }
  printf("Divisor: %d\n", kernel->divisor);
}

/*

free's the kernel data and pointer malloc'ed

 */

void free_kernel(Kernel *kernel) {
  if (kernel != NULL) {
    free(kernel->data);
    free(kernel);
  }
}
