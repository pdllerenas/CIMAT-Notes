#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/linear_system.h"
#include "../../utils/linalg/matrix.h"
#include "../../utils/linalg/matrix_factorization.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/vector.h"
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
  double lambda = iterative_power(A, b, 1e-6, 100);
  // Using A = LU, we have LUx = b, so we solve Ly = b and then Ux = y.
  printf("%lf\n", lambda);
  matrix_free(A);
  free_vector(b);
  return 0;
}
