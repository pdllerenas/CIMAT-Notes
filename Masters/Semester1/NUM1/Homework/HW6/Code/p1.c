#include <stdio.h>
#include <stdlib.h>

#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/matrix.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/matrix_operations.h"

int main(int argc, char *argv[]) {
  if (argc != 8) {
    fprintf(stderr,
            "Usage: %s <matrix-input> <vector-input> <m-values> <TOLERANCE> "
            "<MAX_ITER> <eigval-out> <eigvec-out>\n",
            argv[0]);
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

  int m = atoi(argv[3]);
  double TOL = strtod(argv[4], NULL);
  int MAX_ITER = atoi(argv[5]);
  const char *eigval_filename = argv[6];
  const char *eigvec_filename = argv[7];

  double *eigenvalues = malloc(m * sizeof(double));
  Vector **eigenvectors = malloc(m * sizeof(Vector *));

  get_m_largest_eigenvalues(A, b, m, TOL, MAX_ITER, eigenvalues, eigenvectors);

  FILE *fval = fopen(eigval_filename, "w");
  FILE *fvec = fopen(eigvec_filename, "w");
  FILE *fAxlx = fopen("verify.txt", "w");

  for (int i = 0; i < m; i++) {
    Vector *Ax = matrix_times_vector(A, eigenvectors[i]);
    Vector *lx = vector_scalar_product(eigenvectors[i], eigenvalues[i]);
    Vector *d = vector_diff(Ax, lx);
    free_vector(Ax);
    free_vector(lx);

    double diff_norm = l2_norm(d);
    free_vector(d);

    fprintf(fAxlx, "%.15lf\n", diff_norm);
    fprintf(fval, "%.15lf\n", eigenvalues[i]);
    for (int j = 0; j < eigenvectors[i]->dim; j++) {
      fprintf(fvec, "%.15lf ", ((double *)eigenvectors[i]->data)[j]);
    }
    fprintf(fvec, "\n");
  }

  fclose(fval);
  fclose(fvec);

  matrix_free(A);
  free_vector(b);
  free(eigenvalues);
  for (int i = 0; i < m; i++)
    free_vector(eigenvectors[i]);
  free(eigenvectors);
  return 0;
}
