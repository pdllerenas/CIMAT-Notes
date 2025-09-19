#include "convolution.h"
#include "compare.h"
#include "kernel_io.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

calculates the (x,y) coordinate of the convolution of (k, matrix)

 */

static int convolution(const Kernel *k, unsigned char **matrix, const int y,
                       const int x, const int w, const int h) {
  int sum = 0;
  for (int i = 0; i < k->dimension; i++) {
    for (int j = 0; j < k->dimension; j++) {
      // matrix offset
      int m_x = x - k->dimension / 2 + j;
      int m_y = y - k->dimension / 2 + i;

      // check if out of matrix bounds
      if (m_x < 0 || m_y < 0 || m_x >= w || m_y >= h) {
        sum += k->data[k->dimension * i + j] * 0;
        continue;
      }
      // if inside matrix, multiply
      sum += k->data[k->dimension * i + j] * matrix[m_y][m_x];
    }
  }
  // divide by kernel divisor. e.g. 3x3 gaussian has divisor 16
  return sum / k->divisor;
}

/*

applies the scharr operators to the given input image

 */
unsigned char **apply_sobel(unsigned char **input, int rows, int cols,
                            Kernel *(kx)(), Kernel *(ky)()) {
  // these call malloc, so we must free them at the end
  Kernel *k_x = kx();
  Kernel *k_y = ky();

  int *gx = malloc(rows * cols * sizeof(int));
  int *gy = malloc(rows * cols * sizeof(int));

  if (!gx || !gy) {
    perror("Could not allocate memory.\n");
    return NULL;
  }

  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  if (!output) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  if (!temp_ptr) {
    perror("Could not allocate memory.\n");
    return NULL;
  }

  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      gx[y * cols + x] = convolution(k_x, input, y, x, rows, cols);
      gy[y * cols + x] = convolution(k_y, input, y, x, rows, cols);
    }
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      // calcuate sum of each operator applied
      int final_val = gx[y * cols + x] + gy[y * cols + x];

      // handle out of bound values
      if (final_val > 255) {
        output[y][x] = 255;
      } else if (final_val < 0) {
        output[y][x] = 0;
      } else {
        output[y][x] = (unsigned char)final_val;
      }
    }
  }
  free(gx);
  free(gy);
  free_kernel(k_x);
  free_kernel(k_y);
  return output;
}

unsigned char **apply_gaussian(unsigned char **input, int rows, int cols,
                               Kernel *(k)()) {
  Kernel *kernel = k();
  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  if (!output) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  if (!temp_ptr) {
    perror("Could not allocate memory.\n");
    return NULL;
  }

  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  // convolution result
  int g;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      g = convolution(kernel, input, y, x, rows, cols);
      if (g > 255) {
        output[y][x] = 255;
      } else if (g < 0) {
        output[y][x] = 0;
      } else {
        output[y][x] = (unsigned char)g;
      }
    }
  }
  free_kernel(kernel);
  return output;
}

/*

applies the mean kernel of dimensions k_dim * k_dim to the given input image

 */

unsigned char **apply_mean(unsigned char **input, int rows, int cols,
                           Kernel *(k)(int), int k_dim) {
  // creates the mean kernel of k_dim * k_dim
  Kernel *kernel = k(k_dim);

  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  if (!output) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  if (!temp_ptr) {
    perror("Could not allocate memory.\n");
    return NULL;
  }

  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      output[y][x] = convolution(kernel, input, y, x, rows, cols);
    }
  }
  free_kernel(kernel);
  return output;
}

/*

calculates the median of a submatrix of input of dimensions
median_dim, assuming the subimatrix is centered at (col = y, row = x)

 */
static unsigned char sub_matrix_median(unsigned char **input, int rows,
                                       int cols, int y, int x, int median_dim) {
  int arr_dim = median_dim * median_dim;

  // helper array to sort and then find median
  unsigned char *sorted_arr = malloc(arr_dim * sizeof(unsigned char));
  if (!sorted_arr) {
    perror("Could not allocate memory.\n");
    exit(1);
  }

  for (int i = 0; i < median_dim; i++) {
    for (int j = 0; j < median_dim; j++) {
      // check out of matrix bounds values,
      // we assume these are black pixels
      if (y + i - 1 < 0 || x + j - 1 < 0 || y + i - 1 >= rows ||
          x + j - 1 >= cols) {
        sorted_arr[i * median_dim + j] = 0;
        continue;
      }
      sorted_arr[i * median_dim + j] = input[y + i - 1][x + j - 1];
    }
  }
  // sort the array by comparing unsigned chars
  qsort(sorted_arr, arr_dim, sizeof(unsigned char), compare_uchar);
  int median = (arr_dim % 2 == 1)
                   ? (sorted_arr[arr_dim / 2])
                   : (sorted_arr[(arr_dim - 1) / 2 + (arr_dim) / 2]);
  free(sorted_arr);
  return median;
}

/*

applies the median filter of size median_dim * median_dim to the input image

 */

unsigned char **median_filter_nxn(unsigned char **input, int rows, int cols,
                                  int median_dim) {
  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  if (!output) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  if (!temp_ptr) {
    perror("Could not allocate memory.\n");
    return NULL;
  }

  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      output[y][x] = sub_matrix_median(input, rows, cols, y, x, median_dim);
    }
  }
  return output;
}

/*

applies the gradient filter to the given image

 */

unsigned char **apply_gradient(unsigned char **input, int rows, int cols) {
  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  if (!output) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  unsigned char *temp_ptr = malloc(rows * cols * sizeof(unsigned char));
  if (!temp_ptr) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  for (int i = 0; i < rows; i++) {
    output[i] = temp_ptr + (i * (cols));
  }
  int x, y;

  for (y = 0; y < rows; y++) {
    for (x = 0; x < cols; x++) {
      int gx, gy;

      // handle horizontal derivative 
      if (x < cols - 1) {
        gx = abs(input[y][x + 1] - input[y][x]);
      } else {
        gx = 0;
      }

      // handle vertical derivative
      if (y < rows - 1) {
        gy = abs(input[y + 1][x] - input[y][x]);
      } else {
        gy = 0;
      }

      // approximation using |gxy| = |gx| + |gy|
      int magnitude = gx + gy;

      // handle out of bounds values
      if (magnitude > 255) {
        output[y][x] = 255;
      } else {
        output[y][x] = (unsigned char)magnitude;
      }
    }
  }
  return output;
}
