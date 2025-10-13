#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/linear_system.h"
#include "../../utils/linalg/matrix.h"
#include "../../utils/linalg/matrix_factorization.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/matrix_operations.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <matrix-input> <vector-input>\n", argv[0]);
    exit(1);
  }

  int rows, cols;
  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  if (!A)
    exit(1);

  Vector *b = load_vector_from_txt(argv[2]);
  if (!b) {
    matrix_free(A);
    exit(1);
  }

  Matrix *Q, *R;
  QR_factorization(A, &Q, &R);
  if (rows < 9) {
    print_matrix(Q);
    print_matrix(R);
  }

  // using Q^-1 = Q^T because Q is orthonormal (by QR method)
  Matrix *QT = matrix_transpose(Q);
  Vector *y = matrix_times_vector(QT, b);

  // using that R is upper triangular
  Vector *x = solve_upper(R, y);
  Vector *Ax = matrix_times_vector(A, x);

  Vector *vy = vector_diff(Ax, b);
  double nvy = l2_norm(vy);

  printf("||Ax-b|| = %.8e\n", nvy);

  matrix_free(A);
  matrix_free(QT);
  free_vector(b);
  matrix_free(Q);
  matrix_free(R);
  free_vector(y);
  free_vector(x);
  free_vector(Ax);
  free_vector(vy);
  return 0;
}
