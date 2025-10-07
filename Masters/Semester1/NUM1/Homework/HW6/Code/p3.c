#include <stdio.h>
#include <stdlib.h>

#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/matrix.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/matrix_operations.h"

int main(int argc, char *argv[]) {

  if (argc != 7) {
    fprintf(stderr,
            "Usage: %s <matrix-input> <phi0> <TOLERANCE> "
            "<MAX_ITER> <eigval-out> <eigvec-out>\n",
            argv[0]);
    exit(1);
  }

  // unused (kept becuase I havent changed the functions)
  int rows, cols;

  double TOL = strtod(argv[3], NULL);
  int MAX_ITER = atoi(argv[4]);
  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  Matrix *phi_0 = load_matrix_from_txt(argv[2], &rows, &cols);
  int m = phi_0->cols;
  const char *eigval_filename = argv[5];
  const char *eigvec_filename = argv[6];

  Matrix *evecs;
  double *evals = subspace_iteration(A, phi_0, TOL, MAX_ITER, &evecs, true);
  FILE *fval = fopen(eigval_filename, "w");
  FILE *fvec = fopen(eigvec_filename, "w");
  FILE *fAxlx = fopen("verify.txt", "w");

  for (int i = 0; i < m; i++) {
    Vector *ei = mcol(evecs, i);
    Vector *Ax = matrix_times_vector(A, ei);
    Vector *lx = vector_scalar_product(ei, evals[i]);
    Vector *d = vector_diff(Ax, lx);
    free_vector(Ax);
    free_vector(lx);

    double diff_norm = l2_norm(d);
    free_vector(d);

    fprintf(fAxlx, "%.15lf\n", diff_norm);
    fprintf(fval, "%.15lf\n", evals[i]);
    for (int j = 0; j < ei->dim; j++) {
      fprintf(fvec, "%.15lf ", ((double *)ei->data)[j]);
    }
    fprintf(fvec, "\n");
    free_vector(ei);
  }

  fclose(fval);
  fclose(fvec);

  matrix_free(A);
  free(evals);
  matrix_free(evecs);
  return 0;
}
