#include "eigenvalues.h"
#include "linear_system.h"
#include "matrix.h"
#include "matrix_io.h"
#include "matrix_operations.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    x_curr = matrix_times_vector(A, x_prev);
    int y_arg_max = vector_arg_max(x_curr);

    // if Ax = lambda x has greatest absolute value 0, eigenvector is 0
    if (fabs(((double *)x_prev->data)[y_arg_max]) < 1e-9) {
      printf("Eigenvector:\n");
      copy_data(*eigenvector, x_curr);
      return 0;
    }
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
      double x_curr_l2 = l2_norm(x_curr);
      vector_scalar_product_inplace(x_curr, 1.0 / x_curr_l2);

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

double wielandt_deflation(const Matrix *A, const Vector *x0, const Vector *v,
                          const double lambda_v, const double TOL,
                          const int MAX_ITER, Vector **v2_out) {

  int n = A->rows;
  if (n != A->cols) {
    fprintf(stderr, "Error: Matrix is not square.\n");
    *v2_out = NULL;
    return NAN;
  }

  int k = vector_arg_max(v);
  double v_k = ((double *)v->data)[k];
  if (fabs(v_k) < 1e-9) {
    fprintf(stderr, "Dominating vector is zero. Method failed.\n");
    v2_out = NULL;
    return NAN;
  }

  double w_k = 1.0 / v_k;
  Matrix *D = matrix_create_double(n, n);
  if (!D) {
    v2_out = NULL;
    return NAN;
  }

  for (int i = 0; i < n; i++) {
    double D_ik = lambda_v * ((double *)v->data)[i] * w_k;
    ((double *)D->data)[i * n + k] = D_ik;
  }

  Matrix *B = matrix_difference(A, D);
  if (!B) {
    matrix_free(D);
    v2_out = NULL;
    return NAN;
  }
  matrix_free(D);

  double mu = iterative_power(B, x0, TOL, MAX_ITER, v2_out);
  matrix_free(B);

  if (mu == 0.0 && v2_out == NULL) {
    fprintf(stderr, "Power iteration failed to converge.\n");
    return NAN;
  }

  return mu;
}

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

double inverse_iterative_power(Matrix *A, Vector *x0, double TOL,
                               int MAX_ITER) {
  int n = x0->dim;
  int k = 1;
  Vector *x_prev = create_vector(n);
  if (!x_prev) {
    exit(1);
  }
  Matrix *A_current = matrix_create_double(A->rows, A->cols);
  copy_matrix(A_current, A);
  int max_idx = vector_arg_max(x_prev);
  vector_scalar_product_inplace(x_prev, ((double *)x_prev->data)[max_idx]);
  double lambda_prev = 0, lambda_curr;
  double error;
  Vector *x_curr;
  while (k <= MAX_ITER) {
    x_curr = cholesky_solve(A, x_prev);
    double numerator = dot(x_prev, x_curr);
    double denominator = dot(x_prev, x_prev);
    if (fabs(denominator) < 1e-9) {
      fprintf(stderr, "Previous vector norm too small.\n");
      free_vector(x_curr);
      free_vector(x_prev);
      return NAN;
    }
    lambda_curr = numerator / denominator;

    error = fabs(lambda_curr - lambda_prev);
    if (error < TOL) {
      printf("Method converged after %d iterations.\n", k + 1);
      free_vector(x_curr);
      free_vector(x_prev);
      return 1 / lambda_curr;
    }

    double x_curr_norm = l2_norm(x_curr);
    vector_scalar_product(x_curr, 1 / x_curr_norm);
    free_vector(x_prev);
    x_prev = x_curr;
    lambda_prev = lambda_curr;
    k++;
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}
