#include "matrix.h"
#include "matrix_operations.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

double iterative_power(Matrix *A, Vector *x, double TOL, int MAX_ITER) {
  int k = 1;

  int x_arg_max = vector_arg_max(x);
  int y_arg_max;
  Vector *y;
  double lambda;
  vector_scalar_product(x, 1 / ((double *)x->data)[x_arg_max]);

  while (k <= MAX_ITER) {
    y = matrix_times_vector(A, x);
    lambda = ((double *)y->data)[x_arg_max];
    y_arg_max = vector_arg_max(y);
    if (y_arg_max == 0) {
      printf("Eigenvector:\n");
      print_vector(x);
      return 0;
    }

    vector_scalar_product(y, 1 / ((double *)y->data)[y_arg_max]);
    double error = max_norm(vector_diff(x, y));
    copy_data(x, y);

    if (error < TOL) {
      return lambda;
    }
    k++;
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}

double inverse_iterative_power(Matrix *A, Vector *x, double TOL, int MAX_ITER) {
  int k = 1;
  vector_scalar_product(x, l2_norm(x));
  Vector *y;
  double lambda, y_norm2;
  while (k <= MAX_ITER) {
    y = matrix_times_vector(A, x);
    lambda = dot(x, y);

    y_norm2 = l2_norm(y);
    if (y_norm2 == 0) {
      return 0;
    }
    vector_scalar_product(y, y_norm2);
    double error = l2_norm(vector_diff(x, y));
    copy_data(x, y);
    if (error < TOL) {
      return lambda;
    }
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  exit(1);
}
