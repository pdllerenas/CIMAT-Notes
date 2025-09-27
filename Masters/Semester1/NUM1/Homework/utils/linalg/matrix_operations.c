#include "matrix_operations.h"
#include "matrix.h"
#include "vector.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*

returns the product of the matrices

 */

Matrix *matrix_product(const Matrix *A, const Matrix *B) {
  if (A->cols != B->rows) {
    fprintf(stderr, "Matrix dimensions mismatch: %d does not equal %d.\n",
            A->cols, B->rows);
    return NULL;
  }
  Matrix *P = matrix_create_double(A->rows, B->cols);
  double sum;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->cols; j++) {
      sum = 0.0;
      for (int k = 0; k < A->cols; k++) {
        sum += ((double *)A->data)[i * A->rows + k] *
               ((double *)B->data)[k * B->rows + j];
      }
      ((double *)P->data)[i * A->rows + j] = sum;
    }
  }
  return P;
}

/*

returns the matrix A+B

 */

Matrix *matrix_sum(const Matrix *A, const Matrix *B) {
  if (A->rows != B->rows || A->cols != B->cols) {
    fprintf(stderr, "Cannot operate matrices. Dimension mismatch\n");
    return NULL;
  }
  Matrix *S = matrix_create_double(A->rows, A->cols);
  if (!S) {
    fprintf(stderr, "Could not create matrix.\n");
    return NULL;
  }
  for (int i = 0; i < S->rows; i++) {
    for (int j = 0; j < S->cols; j++) {
      ((double *)S->data)[i * S->rows + j] =
          ((double *)A->data)[i * A->rows + j] +
          ((double *)B->data)[i * B->rows + j];
    }
  }
  return S;
}

/*

returns the matrix A - B

 */

Matrix *matrix_difference(const Matrix *A, const Matrix *B) {
  if (A->rows != B->rows || A->cols != B->cols) {
    fprintf(stderr, "Cannot operate matrices. Dimension mismatch\n");
    return NULL;
  }
  Matrix *D = matrix_create_double(A->rows, A->cols);
  if (!D) {
    fprintf(stderr, "Could not create matrix.\n");
    return NULL;
  }
  for (int i = 0; i < D->rows; i++) {
    for (int j = 0; j < D->cols; j++) {
      ((double *)D->data)[i * D->rows + j] =
          ((double *)A->data)[i * A->rows + j] -
          ((double *)B->data)[i * B->rows + j];
    }
  }
  return D;
}

/*

returns the transposed matrix as a new matrix

 */

Matrix *matrix_transpose(const Matrix *A) {
  Matrix *T = matrix_create_double(A->cols, A->rows);
  for (int i = 0; i < T->rows; i++) {
    for (int j = 0; j < T->cols; j++) {
      ((double *)T->data)[i * T->rows + j] =
          ((double *)A->data)[j * A->rows + i];
    }
  }
  return T;
}

/*

returns -1 if not comparable (so no, they are not equal)
returns 0 if equal
returns 1 if different (and comparable)

 */
int matrix_compare(const Matrix *A, const Matrix *B, double TOL) {
  if (A->rows != B->rows || A->cols != B->cols) {
    fprintf(stderr, "Cannot comapre matrices. Dimension mismatch\n");
    return -1;
  }
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->cols; j++) {
      if (fabs(((double *)A->data)[i * A->rows + j] -
               ((double *)B->data)[i * B->rows + j]) > TOL) {
        return 1;
      }
    }
  }
  return 0;
}

/*

computes M x and returns the result as a new vector.

*/
Vector *matrix_times_vector(const Matrix *M, const Vector *x) {
  if (M->cols != x->dim) {
    fprintf(stderr, "Dimension mismatch: %d does not equal %d\n", M->cols,
            x->dim);
    return NULL;
  }
  Vector *Mx = create_vector(M->rows);
  double sum;
  for (int i = 0; i < M->rows; i++) {
    sum = 0.0;
    for (int j = 0; j < M->cols; j++) {
      sum += ((double *)M->data)[i * M->cols + j] * ((double *)x->data)[j];
    }
    ((double *)Mx->data)[i] = sum;
  }
  return Mx;
}

void matrix_swap_rows(Matrix *M, const int i, const int j) {
  int dim = M->cols;
  double temp;
  for (int k = 0; k < dim; k++) {
    temp = ((double *)M->data)[i * dim + k];
    ((double *)M->data)[i * dim + k] = ((double *)M->data)[j * dim + k];
    ((double *)M->data)[j * dim + k] = temp;
  }
}

Matrix *deflation_term(const Vector *v, double lambda_v) {
  int n = v->dim;
  int k = vector_arg_max(v);

  double v_k = ((double *)v->data)[k];
  if (fabs(v_k) < 1e-9) {
    fprintf(stderr, "Error: vector near zero. Method fails.\n");
  }
  double w_k = 1.0/v_k;

  Matrix *D = matrix_create_double(n, n);
  if (!D) {
    return NULL;
  }

  for (int i = 0; i < n; i++) {
    double D_ik = lambda_v * ((double *)v->data)[i] * w_k;
    ((double *)D->data)[i*n+k] = D_ik;
  }
  return D;
}
