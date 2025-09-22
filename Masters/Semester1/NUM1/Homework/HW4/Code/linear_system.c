#include "linear_system.h"
#include "matrix.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

Vector *solve_diagonal(Matrix *D, Vector *b) {
  if (D->rows != b->dim) {
    fprintf(stderr,
            "Invalid system of equations.\nDimension mismatch: %d does not "
            "equal %d.\n",
            D->rows, b->dim);
    return NULL;
  }
  Vector *x = create_vector(D->cols);
  if (!x) {
    fprintf(stderr, "Unable to solve system of equations.\n");
    return NULL;
  }
  for (int i = 0; i < x->dim; i++) {
    ((double *)x->data)[i] = ((double *)b->data)[i] / ((double *)D->data)[i];
  }
  return x;
}

Vector *solve_upper(Matrix *U, Vector *b) {
  if (U->rows != b->dim) {
    fprintf(stderr,
            "Invalid system of equations.\nDimension mismatch: %d does not "
            "equal %d.\n",
            U->rows, b->dim);
    return NULL;
  }
  Vector *x = create_vector(U->cols);
  if (!x) {
    fprintf(stderr, "Unable to solve system of equations.\n");
    return NULL;
  }

  // if U[n][n] = 0, exit process
  if (((double *)U->data)[U->rows * U->cols - 1] == 0) {
    fprintf(stderr, "Unique solution does not exist.\n");
    return NULL;
  }

  // set x[n] = b[n]/U[n]
  ((double *)x->data)[x->dim - 1] = ((double *)b->data)[x->dim - 1] /
                                    ((double *)U->data)[U->rows * U->cols - 1];
  double sum;
  for (int i = x->dim - 2; i >= 0; i--) {
    // check if diagonal value is 0
    if (((double *)U->data)[i * U->cols + i] == 0) {
      fprintf(stderr, "Unique solution does not exist.\n");
      return NULL;
    }

    sum = 0.0;
    for (int k = i + 1; k < U->cols; k++) {
      sum += ((double *)x->data)[k] * ((double *)U->data)[i * U->cols + k];
    }
    ((double *)x->data)[i] =
        (((double *)b->data)[i] - sum) / ((double *)U->data)[i * U->cols + i];
  }
  return x;
}

Vector *solve_lower(Matrix *L, Vector *b) {
  if (L->rows != b->dim) {
    fprintf(stderr,
            "Invalid system of equations.\nDimension mismatch: %d does not "
            "equal %d.\n",
            L->rows, b->dim);
    return NULL;
  }
  Vector *x = create_vector(L->cols);
  if (!x) {
    fprintf(stderr, "Unable to solve system of equations.\n");
    return NULL;
  }
  if (((double *)L->data)[0] == 0) {
    fprintf(stderr, "System has no unique solution.\n");
    return NULL;
  }
  ((double *)x->data)[0] = ((double *)b->data)[0] /
                           ((double *)L->data)[0]; // first element of solution
  for (int i = 1; i < x->dim; i++) {
    if (((double *)L->data) == 0) {
      fprintf(stderr, "System has no unique solution.\n");
      return NULL;
    }
    double sum = 0.0;
    for (int k = 0; k < i; k++) {
      sum += ((double *)x->data)[k] * ((double *)L->data)[i * L->cols + k];
    }
    ((double *)x->data)[i] =
        (((double *)b->data)[i] - sum) / ((double *)L->data)[i * L->cols + i];
  }
  return x;
}

/*

iterative method that finds an approximation to the solution of Ax = b given an
initial guess vector x0.

 */
Vector *jacobi_iterative(Matrix *A, Vector *b, Vector *x0, double TOL,
                         int MAX_ITER) {
  int k = 1;
  int n = b->dim;
  Vector *x_next = create_vector(A->cols);
  Vector *x_prev = x0;
  FILE *diff = fopen("lin_sys/jacobi_convergence.txt", "w");
  while (k <= MAX_ITER) {
    for (int i = 0; i < n; i++) {
      double sum = 0.0;
      for (int j = 0; j < n; j++) {
        if (i == j) {
          continue;
        }
        sum +=
            ((double *)A->data)[i * A->cols + j] * ((double *)x_prev->data)[j];
      }
      // x[i] = (b[i] - sum)/A[i][i]
      ((double *)x_next->data)[i] = (((double *)b->data)[i] - sum) /
                                    (((double *)A->data)[i * A->cols + i]);
    }
    fprintf(diff, "%d %.15lf\n", k,
            vector_norm_squared(vector_diff(x_next, x_prev)));
    if (vector_norm_squared(vector_diff(x_next, x_prev)) < TOL) {
      return x_next;
    }
    k++;
    copy_data(x_prev, x_next);
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  return NULL;
}

/*

iterative method that finds an approximation to the solution of Ax = b given an
initial guess vector x0. This method differs from the jacobi method in the
calculation of x[i]. In the Jacobi method we exclusively use values from the
previous vector, while in the gauss_seidel method, we also use values from the
current x[i].

 */
Vector *gauss_seidel_iterative(Matrix *A, Vector *b, Vector *x0, double TOL,
                               int MAX_ITER) {
  int k = 1;
  int n = b->dim;
  Vector *x_next = create_vector(A->cols);
  Vector *x_prev = x0;
  FILE *diff = fopen("lin_sys/gaussian_seidel_convergence.txt", "w");

  while (k <= MAX_ITER) {
    for (int i = 0; i < n; i++) {
      double sum = 0.0;
      for (int j = 0; j < n; j++) {
        if (i == j) {
          continue;
        } else if (j > i) {
          sum += ((double *)A->data)[i * A->cols + j] *
                 ((double *)x_prev->data)[j];
        } else if (j < i) {
          sum += ((double *)A->data)[i * A->cols + j] *
                 ((double *)x_next->data)[j];
        }
      }
      ((double *)x_next->data)[i] = (((double *)b->data)[i] - sum) /
                                    (((double *)A->data)[i * A->cols + i]);
    }

    fprintf(diff, "%d %.15lf\n", k,
            vector_norm_squared(vector_diff(x_next, x_prev)));
    if (vector_norm_squared(vector_diff(x_next, x_prev)) < TOL) {
      return x_next;
    }
    k++;
    copy_data(x_prev, x_next);
  }
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  return NULL;
}
