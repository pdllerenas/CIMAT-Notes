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
        ndata[i * 2000 + j] = 40;
        fprintf(stream, "40");
      } else if (j == i + 1) {
        fprintf(stream, "-8");
        ndata[i * 2000 + j] = -8;
        ndata[j * 2000 + i] = -8;
      } else if (j == i - 1) {
        fprintf(stream, "-8");
      } else if (j == i + 2) {
        ndata[i * 2000 + j] = -4;
        ndata[j * 2000 + i] = -4;
        fprintf(stream, "-4");
      } else if (j == i - 2) {
        fprintf(stream, "-4");
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
  fprintf(stream, "20\n");
  fprintf(stream, "50\n");
  vdata[0] = 20;
  vdata[1] = 50;
  for (int i = 2; i < 1998; i++) {
    vdata[i] = 100;
    fprintf(stream, "100\n");
  }
  fprintf(stream, "50\n");
  fprintf(stream, "20");
  vdata[1998] = 50;
  vdata[1999] = 20;
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

Matrix *generate_phi0(int n, int m) {
  srand(2025);

  Matrix *phi0 = matrix_create_double(n, m);
  double *phidata = (double *)phi0->data;
  for (int i = 0; i < m; i ++) {
    phidata[i * m + i] = 1.0;
  }

  for (int i = m; i < n; i++) {
    for (int j = 0; j < m; j++) {
      phidata[i * m + j] = (double)rand() / RAND_MAX; 
    }
  }
  return phi0;
}

int main(int argc, char *argv[]) {
  float sTime = (float)clock() / CLOCKS_PER_SEC;
  FILE *fp = fopen("matrix.txt", "w");
  FILE *fpv = fopen("vector.txt", "w");

  Matrix *n_diag = generate_matrix(fp);
  Matrix *L = matrix_create_double(2000, 2000);
  Vector *b = generate_vector(fpv);
  Vector *x0 = init_rvector(2000);
  // cholesky(n_diag, L, 2000);
  // // Vector *x = cholesky_solve(L, b);
  // Vector *x = conjugate_gradient(n_diag, b, x0, 1e-12, 100);
  // float eTime = (float)clock() / CLOCKS_PER_SEC;
  // Vector *Ax = matrix_times_vector(n_diag, x);
  // Vector *r = vector_diff(Ax, b);
  // printf("||r|| = %.6e\n", l2_norm(r));
  // printf("||r||/||b|| = %.6e\n", l2_norm(r)/l2_norm(b));
  // printf("Execution time: %lf ms\n", (eTime - sTime) * 1000);
  // Vector *x = jacobi_iterative(n_diag, b, x0, 1e-12, 10000);
  // print_vector(x);

  double *eigenvalues = malloc(10 * sizeof(double));
  // Vector **eigenvectors = malloc(10 * sizeof(Vector *));
  // for (int i = 0; i < 10; i++) {
  //   eigenvectors[i] = create_vector(2000);
  // }

  // get_m_smallest_eigenvalues(n_diag, b, 4, 1e-2, 10000, eigenvalues,
  //                            eigenvectors);
  Matrix *evecs;
  Matrix *phi0 = generate_phi0(n_diag->cols, 10);
  eigenvalues = subspace_iteration(n_diag, phi0, 1e-5, 10000, &evecs, true);
  for (int i = 0; i < 10; i++) {
    printf("%lf\n", eigenvalues[i]);
  }

  free_vector(b);
  // free(eigenvalues);
  // for (int i = 0; i < 10; i++) {
  //   free_vector(eigenvectors[i]);
  // }
  // free(eigenvectors);
  // free_vector(x);
  matrix_free(n_diag);
  matrix_free(L);
  free(eigenvalues);
  matrix_free(evecs);
  matrix_free(phi0);
  fclose(fp);
  fclose(fpv);
}
