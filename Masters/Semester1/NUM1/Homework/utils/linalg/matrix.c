#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "vector.h"

#define DEFINE_MATRIX_FUNCTIONS(TYPE, SUFFIX)                                  \
  Matrix *matrix_create_##SUFFIX(int rows, int cols) {                         \
    Matrix *m = malloc(sizeof(Matrix));                                        \
    if (m == NULL) {                                                           \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    m->rows = rows;                                                            \
    m->cols = cols;                                                            \
    m->element_size = sizeof(TYPE);                                            \
    m->data = calloc(rows * cols, sizeof(TYPE));                               \
    if (m->data == NULL) {                                                     \
      free(m);                                                                 \
      return NULL;                                                             \
    }                                                                          \
    return m;                                                                  \
  }                                                                            \
                                                                               \
  TYPE matrix_get_##SUFFIX(const Matrix *m, int row, int col) {                \
    if (m == NULL || m->data == NULL) {                                        \
      fprintf(stderr, "Error: Matrix or data is null.\n");                     \
      return (TYPE)0;                                                          \
    }                                                                          \
    return ((TYPE *)m->data)[row * m->cols + col];                             \
  }                                                                            \
                                                                               \
  void matrix_set_##SUFFIX(Matrix *m, int row, int col, TYPE value) {          \
    if (m == NULL || m->data == NULL) {                                        \
      fprintf(stderr, "Error: Matrix or data is null.\n");                     \
      return;                                                                  \
    }                                                                          \
    ((TYPE *)m->data)[row * m->cols + col] = value;                            \
  }

#define DEFINE_NON_CONTIGUOUS_MATRIX_FUNCTIONS(TYPE, SUFFIX)                   \
  TYPE **special_matrix_create_##SUFFIX(int dimension, MatrixType type) {      \
    TYPE **matrix = malloc(dimension * sizeof(double));                        \
    if (matrix == NULL) {                                                      \
      perror("Failed to allocate row pointers.\n");                            \
      return NULL;                                                             \
    }                                                                          \
    for (int i = 0; i < dimension; i++) {                                      \
      int cols_to_alloc = 0;                                                   \
      switch (type) {                                                          \
      case MATRIX_UPPER_TRIANGULAR:                                            \
        cols_to_alloc = dimension - i;                                         \
        break;                                                                 \
      case MATRIX_LOWER_TRIANGULAR:                                            \
        cols_to_alloc = i + 1;                                                 \
        break;                                                                 \
      case MATRIX_DIAGONAL:                                                    \
        cols_to_alloc = 1;                                                     \
        break;                                                                 \
      case MATRIX_FULL:                                                        \
      default:                                                                 \
        cols_to_alloc = dimension;                                             \
      }                                                                        \
      matrix[i] = calloc(cols_to_alloc, sizeof(TYPE));                         \
      if (!matrix[i]) {                                                        \
        for (int j = 0; j < i; j++) {                                          \
          free(matrix[j]);                                                     \
        }                                                                      \
        free(matrix);                                                          \
        return NULL;                                                           \
      }                                                                        \
    }                                                                          \
    return matrix;                                                             \
  }

DEFINE_MATRIX_FUNCTIONS(double, double)
DEFINE_MATRIX_FUNCTIONS(float, float)
DEFINE_MATRIX_FUNCTIONS(int, int)

DEFINE_NON_CONTIGUOUS_MATRIX_FUNCTIONS(double, double)
DEFINE_NON_CONTIGUOUS_MATRIX_FUNCTIONS(float, float)
DEFINE_NON_CONTIGUOUS_MATRIX_FUNCTIONS(int, int)

void copy_matrix(Matrix *m1, const Matrix *m2) {
  if (m1->cols != m2->cols || m1->rows != m2->rows) {
    fprintf(stderr, "Dimension mismatch while copying contents of matrix\n");
    return;
  }
  int cols = m2->cols;
  for (int i = 0; i < m2->rows; i++) {
    for (int j = 0; j < m2->cols; j++) {
      ((double *)m1->data)[i * cols + j] = ((double *)m2->data)[i * cols + j];
    }
  }
}

void matrix_free(Matrix *m) {
  if (m != NULL) {
    free(m->data);
    free(m);
  }
}

Matrix *identity_matrix(int n) {
  Matrix *I = matrix_create_double(n, n);
  for (int i = 0; i < n; i++) {
    ((double *)I->data)[i * n + i] = 1.0;
  }
  return I;
}

Matrix *fill_diagonal_matrix(double v, int n) {
  Matrix *I = matrix_create_double(n, n);
  for (int i = 0; i < n; i++) {
    ((double *)I->data)[i * n + i] = v;
  }
  return I;
}

Matrix *matrix_minor(const Matrix *A, int d) {
  int c = A->cols;
  Matrix *m = matrix_create_double(A->rows, A->cols);
  double *mdata = (double *)m->data;
  double *adata = (double *)A->data;

  for (int i = 0; i < d; i++) {
    mdata[i * c + i] = 1;
  }
  for (int i = d; i < A->rows; i++) {
    for (int j = d; j < A->cols; j++) {
      mdata[i * c + j] = adata[i * c + j];
    }
  }
  return m;
}

/* take c-th column of m, put in v */
Vector *mcol(Matrix *m, int c) {
  Vector *v = create_vector(m->rows);
  if (!v) {
    return NULL;
  }
  double *mdata = (double *)m->data;
  double *vdata = (double *)v->data;

  for (int i = 0; i < m->rows; i++) {
    vdata[i] = mdata[i * m->cols + c];
  }

  return v;
}

/* m = I - v v^T */
Matrix *vmul(Vector *v) {
  int n = v->dim;
  Matrix *x = matrix_create_double(n, n);
  double *xdata = (double *)x->data;
  double *vdata = (double *)v->data;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      xdata[i * n + j] = -2 * vdata[i] * vdata[j];
  for (int i = 0; i < n; i++)
    xdata[i * n + i] += 1;

  return x;
}

Matrix *matrix_deep_copy(const Matrix *A) {
  Matrix *cpy = matrix_create_double(A->rows, A->cols);
  copy_matrix(cpy, A);

  return cpy;
}
