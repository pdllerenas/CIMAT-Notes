#include "eigenvalues.h"
#include "linear_system.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_operations.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*

finds the biggest eigenvalue and stores its eigenvector in eigenvector

 */

double iterative_power(const Matrix *A, const Vector *x0, const double TOL,
                       const int MAX_ITER, Vector **eigenvector) {
  int n = x0->dim;
  int k = 1;
  Vector *x_prev = create_vector(n);
  if (!x_prev) {
    exit(1);
  }
  copy_data(x_prev, x0);

  Vector *x_curr;
  double error, lambda_prev = 0.0, lambda_curr;

  while (k <= MAX_ITER) {
    // xn = Axn-1
    x_curr = matrix_times_vector(A, x_prev);
    int y_arg_max = vector_arg_max(x_curr);

    // if Ax = lambda x has greatest absolute value 0, eigenvector is 0
    if (fabs(((double *)x_prev->data)[y_arg_max]) < 1e-9) {
      printf("Eigenvector:\n");
      copy_data(*eigenvector, x_curr);
      return 0;
    }
    // eigenvalue approximation
    lambda_curr = ((double *)x_curr->data)[y_arg_max] /
                  ((double *)x_prev->data)[y_arg_max];

    error = fabs(lambda_curr - lambda_prev);
    if (error < TOL) {
      printf("Method converged after %d iterations.\n", k + 1);
      if (*eigenvector == NULL) {
        *eigenvector = create_vector(n);
        if (!*eigenvector) {
          free_vector(x_prev);
          free_vector(x_curr);
          return NAN;
        }
      }
      // normalization
      double x_curr_l2 = l2_norm(x_curr);
      vector_scalar_product_inplace(x_curr, 1.0 / x_curr_l2);

      // copy data to eigenvector
      copy_data(*eigenvector, x_curr);
      free_vector(x_prev);
      free_vector(x_curr);
      return lambda_curr;
    }
    double max_val = ((double *)x_curr->data)[y_arg_max];

    // update x_curr with its normalization
    vector_scalar_product_inplace(x_curr, 1.0 / max_val);
    copy_data(x_prev, x_curr);

    lambda_prev = lambda_curr;
    k++;
    free_vector(x_curr);
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}

/*

finds the m biggest eigenvalues of A, stored in eigenvalues_out and its
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
  if (!A_current) {
    return;
  }
  copy_matrix(A_current, A);
  Vector *x_start = create_vector(x0->dim);
  if (!x_start) {
    matrix_free(A_current);
    return;
  }
  copy_data(x_start, x0);

  for (int i = 0; i < m; i++) {
    double lambda_i;
    Vector *v_i = NULL;

    lambda_i = iterative_power(A_current, x_start, TOL, MAX_ITER, &v_i);
    if (!v_i) {
      fprintf(stderr, "Failed to find %d biggest eigenvalue.\n", i);
      matrix_free(A_current);
      free_vector(x_start);
      eigenvalues_out[i] = 0.0;
      eigenvectors_out[i] = NULL;
      break;
    }

    eigenvalues_out[i] = lambda_i;
    eigenvectors_out[i] = v_i;

    if (i < m - 1) {
      Matrix *D = deflation_term(v_i, lambda_i);
      Matrix *A_next = matrix_difference(A_current, D);

      matrix_free(A_current);
      matrix_free(D);

      A_current = A_next;
    } else {
      break;
    }
  }

  if (A_current) {
    matrix_free(A_current);
  }
  if (x_start) {
    free_vector(x_start);
  }
}

/*

single iteration for finding smallest eigenvalue. Takes additional parameters
for m-iterations for orthogonalization of next eigenvectors.

A is the target matrix.
x0 is our initial iterating vector.
TOL max tolerance for stopping algorithm.
MAX_ITER max iterations available.
v_i eigenvector for the found eigenvalue.
prev_vecs list of previously found eigenvectors for next vector.
orthogonalization found_count counter for previous vectors, to know how many
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

finds the m smallest eigenvalues of A, modifies the pointer eigenvales_out and
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
