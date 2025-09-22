#include "linear_system.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_io.h"
#include "matrix_operations.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 7) {
    fprintf(stderr, "Invalid argument count. \nUsage: %s <Length> <Konstant> <Q> <phi_0> <phi_n> <n>",
            argv[0]);
    exit(1);
  }
  double length = strtod(argv[1], NULL);
  int n = atoi(argv[6]);
  double Q = strtod(argv[3], NULL);
  double phi_0 = strtod(argv[4], NULL);
  double phi_n = strtod(argv[5], NULL);
  double K = strtod(argv[2], NULL);

  // returns an n x 2 matrix containing the main diagonal and the one-off diagonal term
  Matrix *A = create_heat_matrix_1d(n);
  if (!A) {
    fprintf(stderr, "Could not execute program.\n");
    exit(1);
  }

  // matrix that will contain the cholesky decomposition
  Matrix *L = matrix_create_double(A->rows, A->rows);
  if (!L) {
    fprintf(stderr, "Could not execute program.\n");
    matrix_free(A);
    exit(1);
  }

  // separation length between partition points
  double dx = length / (n+1);
  cholesky_symmetric_banded(A, L);

  // create an n-dimensional vector with all entries equal to the term Q*dx^2/K
  Vector *v_Q = vector_fill(n, Q*dx*dx/K);
  if (!v_Q) {
    matrix_free(L);
    matrix_free(A);
    exit(1);
  }

  // set initial conditions on phi
  ((double *)v_Q->data)[0] += phi_0;
  ((double *)v_Q->data)[v_Q->dim - 1] += phi_n;

  // using cholesky decomposition,
  // first solve Ly = b, then
  // L^Tx = y.
  Vector *y = solve_lower(L, v_Q);
  if (!y) {
    matrix_free(L);
    matrix_free(A);
    free_vector(v_Q);
    exit(1);
  }
  Matrix *LT = matrix_transpose(L);
  if (!LT) {
    matrix_free(L);
    matrix_free(A);
    free_vector(v_Q);
    free_vector(y);
    exit(1);
  }
  Vector *x = solve_upper(LT, y);
  if (!x) {
    matrix_free(L);
    matrix_free(A);
    free_vector(v_Q);
    free_vector(y);
    matrix_free(LT);
    exit(1);
  }
  
  
  // print resulting phi vector
  FILE *fpx = fopen("lin_sys/phi.txt", "w");
  for (int i = 0; i < x->dim - 1; i++) {
    fprintf(fpx, "%lf\n", ((double *)x->data)[i]);
  }
  fprintf(fpx, "%lf", ((double *)x->data)[x->dim - 1]);

  free_vector(y);
  free_vector(v_Q);
  free_vector(x);
  matrix_free(L);
  matrix_free(LT);
  matrix_free(A);
  return 0;
}
