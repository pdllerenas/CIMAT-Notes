#pragma once

#include <stddef.h>

typedef enum {
  MATRIX_FULL,
  MATRIX_DIAGONAL,
  MATRIX_UPPER_TRIANGULAR,
  MATRIX_LOWER_TRIANGULAR,
} MatrixType;

typedef struct {
  int rows;
  int cols;
  size_t element_size;
  void* data;
} Matrix;

Matrix* matrix_create_double(int rows, int cols);
double matrix_get_double(const Matrix* m, int row, int col);
void matrix_set_double(Matrix* m, int row, int col, double value);

Matrix* matrix_create_int(int rows, int cols);
int matrix_get_int(const Matrix* m, int row, int col);
void matrix_set_int(Matrix* m, int row, int col, int value);

Matrix* matrix_create_float(int rows, int cols);
float matrix_get_float(const Matrix* m, int row, int col);
void matrix_set_float(Matrix* m, int row, int col, float value);

void matrix_free(Matrix* m);

double **matrix_create_double_non_contiguous(int rows, int cols, MatrixType type);
int **matrix_create_int_non_contiguous(int rows, int cols, MatrixType type);
float **matrix_create_float_non_contiguous(int rows, int cols, MatrixType type);

double **special_matrix_create_double(int dimension, MatrixType type);
int **special_matrix_create_int(int dimension, MatrixType type);
float **special_matrix_create_float(int dimension, MatrixType type);
