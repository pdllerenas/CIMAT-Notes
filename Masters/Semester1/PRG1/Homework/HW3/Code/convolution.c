#include "convolution.h"
#include "kernel_io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int convolution(const Kernel *k, unsigned char **matrix, const int x,
                       const int y, const int w, const int h) {
  int sum = 0;
  for (int i = 0; i < k->dimension; i++) {
    for (int j = 0; j < k->dimension; j++) {
      int m_x = x - k->dimension / 2 + j;
      int m_y = y - k->dimension / 2 + i;
      if (m_x < 0 || m_y < 0 || m_x >= h || m_y >= w) {
        sum += k->data[k->dimension * i + j] * 127;
        continue;
      }
      sum += k->data[k->dimension * i + j] * matrix[m_y][m_x];
    }
  }
  return sum / k->divisor;
}

unsigned char **apply_sobel(unsigned char **input, int rows, int cols,
                             Kernel *(kx)(), Kernel*(ky)()) {
  Kernel *k_x = kx();
  Kernel *k_y = ky();
  unsigned char **output = malloc(rows * sizeof(unsigned char *));

  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      output[y][x] = abs(convolution(k_x, input, x, y, rows, cols)) + abs(convolution(k_y, input, x, y, rows, cols));
    }
  }
  free_kernel(k_x);
  free_kernel(k_y);
  return output;
}

unsigned char **apply_filter(unsigned char **input, int rows, int cols,
                             Kernel *(k)()) {
  Kernel *kernel = k();
  unsigned char **output = malloc(rows * sizeof(unsigned char *));

  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      output[y][x] = convolution(kernel, input, x, y, rows, cols);
    }
  }
  free_kernel(kernel);
  return output;
}

unsigned char **gradient_filter(unsigned char **input, int rows, int cols) {
  unsigned char **output = malloc(rows * sizeof(unsigned char *));

  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  for (int y = 0; y < rows - 1; y++) {
    for (int x = 0; x < cols - 1; x++) {
      output[y][x] = abs(input[y][x] - input[y+1][x]) + abs(input[y][x+1] - input[y][x]);
    }
  }
  return output;
}



double *gradient_f(double (*f)(double *, int), double *p, const int dim) {
  double *G = malloc(dim * sizeof(double));
  if (!G) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  double *cpy_p = malloc(dim * sizeof(double));
  if (!cpy_p) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  memcpy(cpy_p, p, dim * sizeof(double));
  for (int i = 0; i < dim; i++) {
    cpy_p[i] = p[i] + 1;
    G[i] = f(p, dim) - f(cpy_p, dim);
    cpy_p[i] = p[i];
  }
  free(cpy_p);
  return G;
}

double l1_norm(double *f, int dim) {
  double sum = 0;
  for (int i = 0; i < dim; i++) {
    sum += fabs(f[i]);
  }
  return sum;
}
