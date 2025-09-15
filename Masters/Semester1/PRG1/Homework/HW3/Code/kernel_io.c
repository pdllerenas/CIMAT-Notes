#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel_io.h"

static const int FILTER_3X3[3 * 3] = {1, 2, 1, 2, 4, 2, 1, 2, 1};

static const int FILTER_5X5[5 * 5] = {1,  4, 7,  4,  1,  4, 16, 26, 16,
                                      4,  7, 26, 41, 26, 7, 4,  16, 26,
                                      16, 4, 1,  4,  7,  4, 1};

static const int FILTER_7X7[7 * 7] = {
    0,  0,  1,  2,  1,  0,  0,  0,   3,  13, 22, 13, 3,  0,  1,  13, 59,
    97, 59, 13, 1,  2,  22, 97, 159, 97, 22, 2,  1,  13, 59, 97, 59, 13,
    1,  0,  3,  13, 22, 13, 3,  0,   0,  0,  1,  2,  1,  0,  0};

static const int SCHARR_H[3 * 3] = {3, 0, -3, 10, 0, -10, 3, 0, -3};

static const int SCHARR_V[3 * 3] = {3, 10, 3, 0, 0, 0, -3, -10, -3};

static const int LAPLACIAN_4[3 * 3] = {0, 1, 0, 1, -4, 1, 0, 1, 0};

static const int LAPLACIAN_20[3 * 3] = {1, 4, 1, 4, -20, 4, 1, 4, 1};

Kernel *create_laplacian_4() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 3;
  k->divisor = 1;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, LAPLACIAN_4, 9 * sizeof(int));
  write_kernel_to_file(k, "laplacian_4.bin");
  return k;
}

Kernel *create_laplacian_20() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 3;
  k->divisor = 1;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, LAPLACIAN_20, 9 * sizeof(int));
  write_kernel_to_file(k, "laplacian_20.bin");
  return k;
}

Kernel *create_horizontal_schar() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 3;
  k->divisor = 1;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, SCHARR_H, 9 * sizeof(int));
  write_kernel_to_file(k, "scharr_h.bin");
  return k;
}

Kernel *create_vertical_schar() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 3;
  k->divisor = 1;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, SCHARR_V, 9 * sizeof(int));
  write_kernel_to_file(k, "scharr_v.bin");
  return k;
}

Kernel *create_mean_3x3() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 3;
  k->divisor = 16;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, FILTER_3X3, 9 * sizeof(int));
  write_kernel_to_file(k, "mean_3x3.bin");
  return k;
}

Kernel *create_mean_5x5() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 5;
  k->divisor = 273;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, FILTER_5X5, 25 * sizeof(int));
  write_kernel_to_file(k, "mean_5x5.bin");
  return k;
}

Kernel *create_mean_7x7() {
  Kernel *k = malloc(sizeof(Kernel));
  if (!k) {
    return NULL;
  }

  k->dimension = 7;
  k->divisor = 1003;
  k->data = malloc(k->dimension * k->dimension * sizeof(int));
  if (!k->data) {
    free(k);
    return NULL;
  }

  memcpy(k->data, FILTER_7X7, 49 * sizeof(int));
  write_kernel_to_file(k, "mean_7x7.bin");
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

void free_kernel(Kernel *kernel) {
  if (kernel != NULL) {
    free(kernel->data);
    free(kernel);
  }
}
