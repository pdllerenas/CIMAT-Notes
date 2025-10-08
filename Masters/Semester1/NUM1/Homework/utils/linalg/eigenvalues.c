#include "eigenvalues.h"
#include "linear_system.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_io.h"
#include "matrix_operations.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

Finds the biggest eigenvalue and stores its eigenvector in eigenvector

 */

double iterative_power(const Matrix *A, const Vector *x0, const double TOL,
                       const int MAX_ITER, Vector **eigenvector) {
  int n = x0->dim;

  Vector *x_prev = create_vector(n);
  copy_data(x_prev, x0);

  Vector *x_curr = NULL;
  double lambda_prev = 0.0, lambda_curr, error;
  int k = 1;

  while (k <= MAX_ITER) {
    x_curr = matrix_times_vector(A, x_prev);

    // Eigenvalue approximation via Rayleigh quotient
    lambda_curr = dot(x_prev, x_curr) / dot(x_prev, x_prev);

    // Normalize x_curr using L2 norm
    double norm = l2_norm(x_curr);
    vector_scalar_product_inplace(x_curr, 1.0 / norm);

    // Check convergence
    error = fabs(lambda_curr - lambda_prev);
    if (error < TOL) {
      if (*eigenvector == NULL) {
        *eigenvector = create_vector(n);
      }
      copy_data(*eigenvector, x_curr);

      free_vector(x_prev);
      free_vector(x_curr);
      printf("Method converged at %d iterations\n", k);
      return lambda_curr;
    }

    copy_data(x_prev, x_curr);
    lambda_prev = lambda_curr;
    free_vector(x_curr);
    x_curr = NULL;
    k++;
  }

  fprintf(stderr, "Power iteration failed to converge after %d iterations.\n",
          MAX_ITER);
  free_vector(x_prev);
  if (x_curr)
    free_vector(x_curr);
  exit(1);
}
/*

Finds the m biggest eigenvalues of A, stored in eigenvalues_out and its
corresponding eigenvectors in eigenvectors_out

 */

void get_m_largest_eigenvalues(const Matrix *A, const Vector *x0,
                               unsigned int m, double TOL, int MAX_ITER,
                               double *eigenvalues_out,
                               Vector **eigenvectors_out) {
  if (m > A->rows) {
    fprintf(stderr, "Error: matrix does not have enough eigenvalues.\n");
    return;
  }

  Matrix *A_current = matrix_create_double(A->rows, A->cols);
  copy_matrix(A_current, A);

  Vector *x_start = create_vector(x0->dim);
  copy_data(x_start, x0);

  for (int i = 0; i < m; i++) {
    Vector *v_i = NULL;
    double lambda_i = iterative_power(A_current, x_start, TOL, MAX_ITER, &v_i);

    eigenvalues_out[i] = lambda_i;
    eigenvectors_out[i] = v_i;

    if (i < m - 1) {
      Matrix *D = deflation_term(v_i, lambda_i);
      Matrix *A_next = matrix_difference(A_current, D);

      matrix_free(D);
      matrix_free(A_current);
      A_current = A_next;

      copy_data(x_start, x0); // reset starting vector
    }
  }

  matrix_free(A_current);
  free_vector(x_start);
}
/*

Single iteration for finding smallest eigenvalue. Takes additional parameters
for m-iterations for orthogonalization of next eigenvectors.

A is the target matrix.
x0 is our initial iterating vector.
TOL max tolerance for stopping algorithm.
MAX_ITER max iterations available.
v_i eigenvector for the found eigenvalue.
Prev_vecs list of previously found eigenvectors for next vector.
Orthogonalization found_count counter for previous vectors, to know how many
times we have to orthogonalize.

 */
double inverse_iterative_power(Matrix *A, Vector *x0, double TOL, int MAX_ITER,
                               Vector **v_i, Vector **prev_vecs,
                               int found_count) {
  int n = x0->dim;
  int k = 1;
  Vector *x_prev = create_vector(n);

  if (!x_prev) {
    exit(1);
  }
  copy_data(x_prev, x0);

  double lambda_prev = 0, lambda_curr;
  double error;

  Vector *x_curr;

  // LU decomposition for solving system of eq.
  Matrix *L = matrix_create_double(A->rows, A->cols);
  Matrix *U = matrix_create_double(A->rows, A->cols);
  crout(A, L, U, A->rows);

  while (k <= MAX_ITER) {
    x_curr = LU_solve(L, U, x_prev);

    // orthogonalization process
    for (int j = 0; j < found_count; j++) {
      double proj = dot(x_curr, prev_vecs[j]);
      vector_axpy_inplace(x_curr, -proj, prev_vecs[j]); // x_curr -= proj * v_j
    }

    // normalization
    double x_curr_norm = l2_norm(x_curr);
    vector_scalar_product_inplace(x_curr, 1.0 / x_curr_norm);

    // eigenvalue approximation
    // q = x^T A x / x^Tx
    Vector *Ax = matrix_times_vector(A, x_curr);
    double numerator = dot(x_curr, Ax);
    double denominator = dot(x_curr, x_curr);
    free_vector(Ax);

    // check if denominator is too small
    if (fabs(denominator) < 1e-9) {
      fprintf(stderr, "Previous vector norm too small.\n");
      matrix_free(L);
      free_vector(x_curr);
      free_vector(x_prev);
      return NAN;
    }
    lambda_curr = numerator / denominator;

    error = fabs(lambda_curr - lambda_prev);
    if (error < TOL) {
      printf("Method converged after %d iterations.\n", k + 1);
      if (*v_i == NULL) {
        *v_i = create_vector(n);
      }
      // store data in eigenvector
      copy_data(*v_i, x_curr);
      matrix_free(L);
      free_vector(x_curr);
      free_vector(x_prev);
      matrix_free(U);
      return lambda_curr;
    }

    // reset for next iteration
    free_vector(x_prev);
    x_prev = x_curr;
    lambda_prev = lambda_curr;
    k++;
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}

/*

Finds the m smallest eigenvalues of A, modifies the pointer eigenvalues_out and
eigenvectors_out with the corresponding values

 */
void get_m_smallest_eigenvalues(const Matrix *A, const Vector *x0,
                                unsigned int m, double TOL, int MAX_ITER,
                                double *eigenvalues_out,
                                Vector **eigenvectors_out) {
  if (m > A->rows) {
    fprintf(stderr, "Error: matrix does not have enough eigenvalues.\n");
    return;
  }
  Vector *x_start = create_vector(x0->dim);
  if (!x_start) {
    return;
  }
  copy_data(x_start, x0);

  for (int i = 0; i < m; i++) {
    double lambda_i;
    Vector *v_i = NULL;

    // pass in previous eigenvectors for orthogonalization
    lambda_i = inverse_iterative_power((Matrix *)A, x_start, TOL, MAX_ITER,
                                       &v_i, eigenvectors_out, i);
    if (!v_i) {
      fprintf(stderr, "Failed to find %d smallest eigenvalue.\n", i);
      eigenvalues_out[i] = 0.0;
      eigenvectors_out[i] = NULL;
      break;
    }

    eigenvalues_out[i] = lambda_i;
    eigenvectors_out[i] = v_i;
  }

  if (x_start) {
    free_vector(x_start);
  }
}

Matrix *jacobi_eigenvalue(Matrix *S, double *eigenvalues, double TOL,
                          int MAX_ITER, int m, bool largest) {
  int n = S->rows;
  if (n != S->cols) {
    fprintf(stderr, "Matrix columns do not match rows.\n");
    return NULL;
  }

  Matrix *A = matrix_deep_copy(S);
  Matrix *E = identity_matrix(n);

  int iter;
  for (iter = 0; iter < MAX_ITER; iter++) {
    double maxval = 0.0;
    int k = 0, l = 1;
    double *adata = (double *)A->data;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        double v = fabs(adata[i * n + j]);
        if (v > maxval) {
          maxval = v;
          k = i;
          l = j;
        }
      }
    }

    if (maxval < TOL)
      break;

    double a_kk = adata[k * n + k];
    double a_ll = adata[l * n + l];
    double p = adata[k * n + l];
    if (p == 0.0)
      continue;

    double theta = (a_ll - a_kk) / (2.0 * p);
    double sign = (theta >= 0.0) ? 1.0 : -1.0;
    double t = sign / (fabs(theta) + sqrt(1.0 + theta * theta));
    double c = 1.0 / sqrt(1.0 + t * t);
    double s = t * c;

    for (int i = 0; i < n; i++) {
      if (i == k || i == l)
        continue;
      double Aik = adata[i * n + k];
      double Ail = adata[i * n + l];
      adata[i * n + k] = c * Aik - s * Ail;
      adata[k * n + i] = adata[i * n + k];
      adata[i * n + l] = s * Aik + c * Ail;
      adata[l * n + i] = adata[i * n + l];
    }

    adata[k * n + k] = a_kk - t * p;
    adata[l * n + l] = a_ll + t * p;
    adata[k * n + l] = 0.0;
    adata[l * n + k] = 0.0;

    double *edata = (double *)E->data;
    for (int i = 0; i < n; i++) {
      double eik = edata[i * n + k];
      double eil = edata[i * n + l];
      edata[i * n + k] = c * eik - s * eil;
      edata[i * n + l] = s * eik + c * eil;
    }
  }

  if (iter == MAX_ITER) {
    fprintf(stderr, "Method failed after %d iterations.\n", iter);
    matrix_free(A);
    matrix_free(E);
    return NULL;
  }

  // Extract all eigenvalues
  double *all_evals = malloc(n * sizeof(double));
  double *adata_final = (double *)A->data;
  for (int i = 0; i < n; i++)
    all_evals[i] = adata_final[i * n + i];

  if (largest == true) {
    // Sort descending and pick top m
    for (int g = 0; g < m; g++) {
      int w = g;
      for (int h = g + 1; h < n; h++)
        if (all_evals[h] > all_evals[w])
          w = h;

      if (w != g) {
        double tmp = all_evals[g];
        all_evals[g] = all_evals[w];
        all_evals[w] = tmp;

        swap_matrix_cols(E, g, w);
      }

      eigenvalues[g] = all_evals[g];
    }
  } else {
    // Sort descending and pick top m
    for (int g = 0; g < m; g++) {
      int w = g;
      for (int h = g + 1; h < n; h++)
        if (all_evals[h] < all_evals[w])
          w = h;

      if (w != g) {
        double tmp = all_evals[g];
        all_evals[g] = all_evals[w];
        all_evals[w] = tmp;

        swap_matrix_cols(E, g, w);
      }

      eigenvalues[g] = all_evals[g];
    }
  }

  free(all_evals);
  matrix_free(A);
  return E;
}

double *subspace_iteration(const Matrix *A, Matrix *Phi_0, double TOL,
                           int MAX_ITER, Matrix **eigenvectors, bool largest) {
  int m = Phi_0->cols;

  Matrix *Phi = matrix_deep_copy(Phi_0);
  Matrix *Q, *R;
  QR_factorization(Phi, &Q, &R);
  matrix_free(Phi);
  Phi = Q;
  matrix_free(R);

  double *eigenvals = malloc(m * sizeof(double));
  double *prev_eigenvals = malloc(m * sizeof(double));

  for (int k = 0; k < MAX_ITER; k++) {
    Matrix *Z = matrix_product(A, Phi);

    QR_factorization(Z, &Q, &R);
    matrix_free(Z);
    matrix_free(R);

    Matrix *B = conjugate_m_by_a(A, Q); // B = Q^T * A * Q

    Matrix *U = jacobi_eigenvalue(B, eigenvals, TOL, MAX_ITER, m, largest);
    matrix_free(B);

    Matrix *Phi_new = matrix_product(Q, U);
    matrix_free(U);
    matrix_free(Q);
    matrix_free(Phi);
    Phi = Phi_new;

    bool converged = true;
    if (k > 0) {
      for (int i = 0; i < m; i++) {
        if (fabs(eigenvals[i] - prev_eigenvals[i]) > TOL) {
          converged = false;
          break;
        }
      }
    }

    if (converged && k > 0) {
      printf("Subspace iteration converged at iteration %d\n", k + 1);
      *eigenvectors = Phi;
      free(prev_eigenvals);
      return eigenvals;
    }

    memcpy(prev_eigenvals, eigenvals, m * sizeof(double));
  }

  fprintf(stderr, "Subspace iteration failed to converge after %d iterations\n",
          MAX_ITER);
  matrix_free(Phi);
  free(eigenvals);
  free(prev_eigenvals);
  return NULL;
}
