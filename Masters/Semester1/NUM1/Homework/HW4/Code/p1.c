#include "linear_system.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_io.h"
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

  Matrix *U = matrix_create_double(A->rows, A->rows);
  if (!U) {
    free_vector(b);
    matrix_free(A);
    exit(1);
  }
  Matrix *L = matrix_create_double(A->rows, A->rows);
  if (!U) {
    free_vector(b);
    matrix_free(A);
    matrix_free(U);
    exit(1);
  }

  // The following function gives A = LU
  crout(A, L, U, A->rows);

  FILE *fpU = fopen("lin_sys/crout_U.txt", "w");
  FILE *fpL = fopen("lin_sys/crout_L.txt", "w");

  for (int i = 0; i < L->rows; i++) {
    for (int j = 0; j < L->rows; j++) {
      fprintf(fpL, "%lf ", ((double *)L->data)[i * rows + j]);
    }
    fprintf(fpL, "\n");
    for (int j = 0; j < U->rows; j++) {
      fprintf(fpU, "%lf ", ((double *)U->data)[i * rows + j]);
    }
    fprintf(fpU, "\n");
  }

  // Using A = LU, we have LUx = b, so we solve Ly = b and then Ux = y.
  Vector *y = solve_lower(L, b);
  if (!y) {
    matrix_free(U);
    matrix_free(L);
    matrix_free(A);
    free_vector(b);
    fclose(fpU);
    fclose(fpL);
  }
  Vector *x = solve_upper(U, y);
  if (!x) {
    matrix_free(U);
    matrix_free(L);
    matrix_free(A);
    free_vector(y);
    free_vector(b);
    fclose(fpU);
    fclose(fpL);
  }
  FILE *fpx = fopen("lin_sys/crout_x.txt", "w");
  for (int i = 0; i < x->dim - 1; i++) {
    fprintf(fpx, "%lf\n", ((double *)x->data)[i]);
  }
  fprintf(fpx, "%lf", ((double *)x->data)[x->dim - 1]);
  matrix_free(U);
  matrix_free(L);
  matrix_free(A);
  free_vector(b);
  free_vector(x);
  free_vector(y);
  fclose(fpU);
  fclose(fpL);
  return 0;
}
