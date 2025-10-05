#pragma once

#include "vector.h"
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
  void *data;
} Matrix;

Matrix *matrix_create_double(int, int);
double matrix_get_double(const Matrix *, int, int);
void matrix_set_double(Matrix *, int, int, double);

Matrix *matrix_create_int(int, int);
int matrix_get_int(const Matrix *, int, int);
void matrix_set_int(Matrix *, int, int, int);

Matrix *matrix_create_float(int, int);
float matrix_get_float(const Matrix *, int, int);
void matrix_set_float(Matrix *, int, int, float);

/*
@return identity matrix of nxn
*/
Matrix *identity_matrix(int n);
Matrix *fill_diagonal_matrix(double v, int n);
Matrix *matrix_minor(const Matrix *A, int d);
Vector* mcol(Matrix *m, int c);
Matrix *vmul(Vector *v);
Matrix *matrix_deep_copy(const Matrix *A);

void copy_matrix(Matrix *, const Matrix *);
void matrix_free(Matrix *);

double **matrix_create_double_non_contiguous(int, int, MatrixType);
int **matrix_create_int_non_contiguous(int, int, MatrixType);
float **matrix_create_float_non_contiguous(int, int, MatrixType);

double **special_matrix_create_double(int, MatrixType);
int **special_matrix_create_int(int, MatrixType);
float **special_matrix_create_float(int, MatrixType);
