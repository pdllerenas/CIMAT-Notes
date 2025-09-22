#include "linear_system.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_io.h"
#include "matrix_operations.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "Invalid argument count. \nUsage: %s <matrix-input> <vector-input>",
            argv[0]);
    exit(1);
  }
  int rows, cols;

  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  if (!A) {
    exit(1);
  }

  Vector *b = load_vector_from_txt(argv[2]);
  if (!b) {
    matrix_free(A);
    exit(1);
  }

  Matrix *L = matrix_create_double(A->rows, A->rows);
  if (!L) {
    free_vector(b);
    matrix_free(A);
    exit(1);
  }

  // The following function gives a matrix L such that A = LL^T
  cholesky(A, L, A->rows);

  FILE *fpLT = fopen("lin_sys/cholesky_LT.txt", "w");
  if (!fpLT) {
    perror("Unable to open file.\n");
    exit(1);
  }
  FILE *fpL = fopen("lin_sys/cholesky_L.txt", "w");
  if (!fpL) {
    perror("Unable to open file.\n");
    exit(1);
  }

  for (int i = 0; i < L->rows; i++) {
    for (int j = 0; j < L->rows; j++) {
      fprintf(fpL, "%lf ", ((double *)L->data)[i * rows + j]);
    }
    fprintf(fpL, "\n");
    for (int j = 0; j < L->rows; j++) {
      fprintf(fpLT, "%lf ", ((double *)L->data)[j * rows + i]);
    }
    fprintf(fpLT, "\n");
  }

  Matrix *LT = matrix_transpose(L);

  // Using A = LL^T, we have LL^Tx = b, so we solve Ly = b and then L^Tx = y.
  Vector *y = solve_lower(L, b);
  if (!y) {
    matrix_free(L);
    matrix_free(LT);
    matrix_free(A);
    free_vector(b);
    fclose(fpLT);
    fclose(fpL);
  }
  Vector *x = solve_upper(LT, y);
  if (!x) {
    matrix_free(LT);
    matrix_free(L);
    matrix_free(A);
    free_vector(y);
    free_vector(b);
    fclose(fpLT);
    fclose(fpL);
  }
  FILE *fpx = fopen("lin_sys/cholesky_x.txt", "w");
  for (int i = 0; i < x->dim - 1; i++) {
    fprintf(fpx, "%lf\n", ((double *)x->data)[i]);
  }
  fprintf(fpx, "%lf", ((double *)x->data)[x->dim - 1]);
  matrix_free(LT);
  matrix_free(L);
  matrix_free(A);
  free_vector(b);
  free_vector(x);
  free_vector(y);
  fclose(fpLT);
  fclose(fpL);
  return 0;
}
