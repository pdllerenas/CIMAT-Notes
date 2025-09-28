#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/matrix.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/vector.h"
#include "../../utils/linalg/linear_system.h"
#include "../../utils/linalg/matrix_factorization.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 6) {
    fprintf(stderr,
            "Invalid argument count. \nUsage: %s <matrix-input> <vector-input> <m-values> <TOLERANCE> <MAX_ITER>",
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

  int m = atoi(argv[3]);
  double TOL = strtod(argv[4], NULL);
  int MAX_ITER = atoi(argv[5]);
  double *eigenvalues = malloc(m * sizeof(double));
  Vector **eigenvector = malloc(m * sizeof(Vector *));

  get_m_smallest_eigenvalues(A, b, m, TOL, MAX_ITER, eigenvalues, eigenvector);

  for (int i = 0; i < m; i++) {
    printf("Eigenvector: ");
    print_vector(eigenvector[i]);

    printf("Eigenvalue: ");
    printf("%lf\n\n", eigenvalues[i]);
  }
  matrix_free(A);
  free_vector(b);
  free(eigenvalues);
  for (int i = 0; i < m; i++) {
    free_vector(eigenvector[i]);
  }
  free(eigenvector);
  return 0;
}
