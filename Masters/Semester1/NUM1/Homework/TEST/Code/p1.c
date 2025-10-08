#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
    for (int j = i; j < 2000; j++) {
      if (i == j) {
        ndata[i * 2000 + j] = 40;
      } else if (j == i + 1) {
        ndata[i * 2000 + j] = -8;
        ndata[j * 2000 + i] = -8;
      } else if (j == i + 2) {
        ndata[i * 2000 + j] = -4;
        ndata[j * 2000 + i] = -4;
      }
    }
    fprintf(stream, "40 -8 -4\n");
  }
  return n_diagonal;
}

Vector *generate_vector() {
  Vector *v = create_vector(2000);
  double *vdata = (double *)v->data;
  vdata[0] = 20;
  vdata[1] = 50;
  for (int i = 2; i < 1998 ; i++) {
    vdata[i] = 100;
  }
  vdata[1999] = 50;
  vdata[2000] = 20;
  return v;
}

Vector* init_rvector(int dim) {

  srand(time(NULL));

  Vector *v = create_vector(dim);
  double *vdata = (double *)v->data;
  for (int i = 0; i < dim; i++) {
    vdata[i] = rand() % 10;
  }
  return v;
}

int main(int argc, char *argv[]) {
  FILE *fp = fopen("matrix.txt", "w");

  Matrix *n_diag = generate_matrix(fp);
  Matrix *L = matrix_create_double(2000, 2000);
  Vector *b = generate_vector();
  cholesky(n_diag, L, 2000);
  // Vector *x = cholesky_solve(L, b);
  Vector *x0 = init_rvector(2000);
  Vector *x = gauss_seidel_iterative(L, b, x0, 1e-12, 10000);

  double *eigenvalues = malloc(10 * sizeof(double));
  Vector *eigenvectors = create_vector(10);

  get_m_largest_eigenvalues(n_diag, b, 10, 1e-6, 10000, eigenvalues, &eigenvectors);
  for (int i = 0; i < 10; i++) {
    printf("%lf\n", eigenvalues[i]);
  }
  
  free_vector(b);
  free_vector(x);
  matrix_free(n_diag);
  matrix_free(L);
}
