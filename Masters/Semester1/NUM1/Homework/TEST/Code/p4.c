#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/linear_system.h"
#include "../../utils/linalg/matrix.h"
#include "../../utils/linalg/matrix_factorization.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/matrix_operations.h"

Matrix *generate_matrix(FILE *stream) {
  Matrix *n_diagonal = matrix_create_double(2000, 2000);
  double *ndata = (double *)n_diagonal->data;
  for (int i = 0; i < 2000; i++) {
    for (int j = 0; j < 2000; j++) {
      if (i == j) {
        ndata[i * 2000 + j] = 50;
        fprintf(stream, "50");
      } else if (j == i + 1) {
        fprintf(stream, "-10");
        ndata[i * 2000 + j] = -10;
        ndata[j * 2000 + i] = -10;
      } else if (j == i - 1) {
        fprintf(stream, "-10");
      } else if (j == i + 2) {
        ndata[i * 2000 + j] = -5;
        ndata[j * 2000 + i] = -5;
        fprintf(stream, "-5");
      } else if (j == i - 2) {
        fprintf(stream, "-5");
      } else {
        fprintf(stream, "0");
      }
      if (j != 1999) {
        fprintf(stream, " ");
      } else {
        fprintf(stream, "\n");
      }
    }
  }
  return n_diagonal;
}

Vector *generate_vector(FILE *stream) {
  Vector *v = create_vector(2000);
  double *vdata = (double *)v->data;
  fprintf(stream, "40\n");
  fprintf(stream, "100\n");
  vdata[0] = 40;
  vdata[1] = 100;
  for (int i = 2; i < 1998; i++) {
    vdata[i] = 200;
    fprintf(stream, "200\n");
  }
  fprintf(stream, "100\n");
  fprintf(stream, "40");
  vdata[1998] = 100;
  vdata[1999] = 40;
  return v;
}

Vector *init_rvector(int dim) {

  srand(time(NULL));

  Vector *v = create_vector(dim);
  double *vdata = (double *)v->data;
  for (int i = 0; i < dim; i++) {
    vdata[i] = rand() % 10;
  }
  return v;
}

void solve_system(Matrix *A, Vector *b, double TOL, int MAX_ITER) {
  Vector *x0 = init_rvector(2000);

  Vector *cx = conjugate_gradient(A, b, x0, 1e-12, 100);
  FILE *fpx_c = fopen("conjugate_s.txt", "w");
  fprint_vector(cx, fpx_c);
  fclose(fpx_c);

  Vector *gx = gauss_seidel_iterative(A, b, x0, 1e-12, 100);
  FILE *fpx_g = fopen("gauss_s.txt", "w");
  fprint_vector(gx, fpx_g);
  fclose(fpx_g);
}

int main(int argc, char *argv[]) {
  FILE *fp = fopen("matrix.txt", "w");
  FILE *fpv = fopen("vector.txt", "w");

  Matrix *n_diag = generate_matrix(fp);
  Vector *b = generate_vector(fpv);

  solve_system(n_diag, b, 1e-12, 10000);

  // smallest eigenvalues
  FILE *fpe1 = fopen("smallest_evals.txt", "w");
  FILE *fpe1vec = fopen("smallest_evec.txt", "w");
  double *eigenvalues = malloc(10 * sizeof(double));
  Vector **eigenvectors = malloc(10 * sizeof(Vector *));
  for (int i = 0; i < 10; i++) {
    eigenvectors[i] = create_vector(2000);
  }

  get_m_smallest_eigenvalues(n_diag, b, 10, 1e-5, 10000, eigenvalues,
                             eigenvectors);
  for (int i = 0; i < 10; i++) {
    fprintf(fpe1, "%lf\n", eigenvalues[i]);
    for (int j = 0; j < 2000; j++) {
      fprintf(fpe1vec, "%lf ", ((double *)eigenvectors[i]->data)[j]);
    }
    fprintf(fpe1vec, "\n");
  }

  // biggest eigenvalues
  // reset eigenvalues
  for (int i = 0; i < 10; i++) {
    eigenvalues[i] = 0.0;
  }
  FILE *fpe2 = fopen("biggest.txt", "w");
  FILE *fpe2vec = fopen("biggest_evec.txt", "w");
  get_m_largest_eigenvalues(n_diag, b, 10, 1e-5, 10000, eigenvalues,
                            eigenvectors);
  for (int i = 0; i < 10; i++) {
    fprintf(fpe2, "%lf\n", eigenvalues[i]);
    fprint_vector(eigenvectors[i], fpe2vec);
    for (int j = 0; j < 2000; j++) {
      fprintf(fpe2vec, "%lf ", ((double *)eigenvectors[i]->data)[j]);
    }
    fprintf(fpe2vec, "\n");
  }

  free_vector(b);
  for (int i = 0; i < 10; i++) {
    free_vector(eigenvectors[i]);
  }
  free(eigenvectors);
  matrix_free(n_diag);
  free(eigenvalues);
  fclose(fp);
  fclose(fpv);
}
