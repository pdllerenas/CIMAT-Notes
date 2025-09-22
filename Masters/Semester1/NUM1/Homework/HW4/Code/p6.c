#include "linear_system.h"
#include "matrix.h"
#include "matrix_io.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 6) {
    fprintf(stderr, "Invalid argument count. \nUsage: %s <A-matrix> <b-vector> <x_0> <TOLERANCE> <MAX_ITER>",
            argv[0]);
    exit(1);
  }
  int rows, cols;

  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  if (!A) {
    fprintf(stderr, "Could not execute program.\n");
    exit(1);
  }
  Vector *b = load_vector_from_txt(argv[2]);
  if (!b) {
    matrix_free(A);
    exit(1);
  }
  Vector *x0 = load_vector_from_txt(argv[3]);
  if (!x0) {
    matrix_free(A);
    free_vector(b);
    exit(1);
  }
  double TOL = strtod(argv[4], NULL);
  int MAX_ITER = atoi(argv[5]);
  
  Vector *x = gauss_seidel_iterative(A, b, x0, TOL, MAX_ITER);
  if (!x) {
    matrix_free(A);
    free_vector(b);
    free_vector(x0);
    exit(1);
  }
  // print solution vector
  FILE *fpx = fopen("lin_sys/gauss_seidel_x.txt", "w");
  for (int i = 0; i < x->dim - 1; i++) {
    fprintf(fpx, "%lf\n", ((double *)x->data)[i]);
  }
  fprintf(fpx, "%lf", ((double *)x->data)[x->dim - 1]);

  free_vector(x);
  free_vector(b);
  free_vector(x0);
  matrix_free(A);
  return 0;
}
