#include "eigenvalues.h"
#include "linear_system.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_operations.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double iterative_power(Matrix *A, Vector *x0, double TOL, int MAX_ITER) {
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
    if (fabs(((double *)x_prev->data)[y_arg_max]) < 1e-9) {
      printf("Eigenvector:\n");
      print_vector(x0);
      return 0;
    }
    lambda_curr = ((double *)x_curr->data)[y_arg_max] /
                  ((double *)x_prev->data)[y_arg_max];

    error = fabs(lambda_curr - lambda_prev);
    if (error < TOL) {
      printf("Method converged after %d iterations.\n", k + 1);
      free_vector(x_prev);
      free_vector(x_curr);
      return lambda_curr;
    }
    double max_val = ((double *)x_curr->data)[y_arg_max];
    vector_scalar_product(x_curr, 1.0 / max_val);
    copy_data(x_prev, x_curr);
    lambda_prev = lambda_curr;

    k++;
    free_vector(x_curr);
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}

double inverse_iterative_power(Matrix *A, Vector *x0, double TOL,
                               int MAX_ITER) {
  int n = x0->dim;
  int k = 1;
  Vector *x_prev = create_vector(n);
  if (!x_prev) {
    exit(1);
  }
  copy_data(x_prev, x0);
  int max_idx = vector_arg_max(x_prev);
  vector_scalar_product(x_prev, ((double*)x_prev->data)[max_idx]);
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
    lambda_curr = numerator/denominator;

    error = fabs(lambda_curr - lambda_prev);
    if (error < TOL) {
      printf("Method converged after %d iterations.\n", k + 1);
      free_vector(x_curr);
      free_vector(x_prev);
      return 1 / lambda_curr;
    }

    double x_curr_norm = l2_norm(x_curr);
    vector_scalar_product(x_curr, 1/x_curr_norm);
    free_vector(x_prev);
    x_prev = x_curr;
    lambda_prev = lambda_curr;
    k++;
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}
