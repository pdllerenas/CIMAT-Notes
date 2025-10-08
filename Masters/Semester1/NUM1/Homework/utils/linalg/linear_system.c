#include "linear_system.h"
#include "matrix.h"
#include "matrix_operations.h"
#include "vector.h"
#include <math.h>
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

Vector *solve_upper_transpose(Matrix *L, Vector *b) {
  int n = L->rows;
  if (L->rows != b->dim) {
    fprintf(stderr,
            "Invalid system of equations.\nDimension mismatch: %d does not "
            "equal %d.\n",
            L->rows, b->dim);
    return NULL;
  }
  Vector *x = create_vector(n);
  if (!x) {
    fprintf(stderr, "Unable to solve system of equations.\n");
    return NULL;
  }
  if (((double *)L->data)[n * n - 1] == 0) {
    fprintf(stderr, "System has no unique solution.\n");
    return NULL;
  }

  for (int i = n - 1; i >= 0; i--) {
    if (((double *)L->data)[i * n + i] == 0) {
      fprintf(stderr, "System has no unique solution.\n");
      return NULL;
    }
    double sum = 0.0;
    for (int k = i + 1; k < n; k++) {
      sum += ((double *)x->data)[k] * ((double *)L->data)[k * n + i];
    }
    ((double *)x->data)[i] =
        (((double *)b->data)[i] - sum) / ((double *)L->data)[i * n + i];
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
    Vector *d = vector_diff(x_next, x_prev);
    if (vector_norm_squared(d) < TOL) {
      free_vector(d);
      return x_next;
    }
    free_vector(d);
    k++;
    copy_data(x_prev, x_next);
  }
  free_vector(x_next);
  fprintf(stderr, "Method failed at %d iterations.\n", k);
  return NULL;
}

/*

modifies the matrix A into row-echelon form via gaussian elimination,
simultaneously changing b's rows, then solve the equivalent problem Ux = b'
using upper solver

 */
Vector *gaussian_elimination(Matrix *A, Vector *b) {
  int dim = A->rows;
  for (int i = 0; i < dim - 1; i++) {
    for (int p = i; p < dim; p++) {
      // if a non-zero entry is found but it is not in the diagonal,
      // swap the m_rows and break
      // (we are looking for the smallest p that satisfies this)
      if (((double *)A->data)[p * dim + i] && p != i) {
        // swap matrix rows
        matrix_swap_rows(A, p, i);

        // swap b rows
        vector_swap_rows(b, p, i);

        break;
      } else if (((double *)A->data)[p * dim + i]) { // if it is already in the
                                                     // diagonal, break
        break;
      }
      // if we have arrived to the final p and no non-zero A[p][i] was found,
      // may conclude that the system has no unique solution.
      else if (p == dim - 1) {
        fprintf(stderr, "Unique solution does not exist.\n");
        exit(1);
      }
    }
    for (int j = i + 1; j < dim; j++) {
      double m =
          ((double *)A->data)[j * dim + i] / ((double *)A->data)[i * dim + i];
      for (int k = 0; k < dim; k++) {
        ((double *)A->data)[j * dim + k] = ((double *)A->data)[j * dim + k] -
                                           m * ((double *)A->data)[i * dim + k];
      }
      ((double *)b->data)[j] -= m * ((double *)b->data)[i];
    }
  }
  for (int i = 0; i < dim; i++) {
    if (((double *)A->data)[i * dim + i] == 0) {
      fprintf(stderr, "Unique solution does not exist.\n");
      exit(1);
    }
  }
  Vector *x = solve_upper(A, b);
  return x;
}

Vector *cholesky_solve(Matrix *L, Vector *b) {
  Vector *y = solve_lower(L, b);

  Vector *x = solve_upper_transpose(L, y);
  free_vector(y);

  return x;
}

Vector *LU_solve(Matrix *L, Matrix *U, Vector *b) {
  Vector *y = solve_lower(L, b);

  Vector *x = solve_upper(U, y);
  free_vector(y);

  return x;
}

Vector *conjugate_gradient(Matrix *A, Vector *b, Vector *x0, double TOL,
                           int MAX_ITER) {
  // TODO: if x0 = 0, rk = b, skipping Ax0
  Vector *Ax0 = matrix_times_vector(A, x0);
  Vector *rk = vector_diff(b, Ax0);
  if (l2_norm(rk) < TOL) {
    return x0;
  }
  free_vector(Ax0);

  Vector *xk = create_vector(x0->dim);
  copy_data(xk, x0);

  Vector *pk = create_vector(rk->dim);
  copy_data(pk, rk);
  int k = 0;

  double rr = dot(rk, rk);

  while (k < MAX_ITER) {
    Vector *Apk = matrix_times_vector(A, pk);
    double denominator = dot(pk, Apk);

    // rk^T rk / pk^T A pk
    double ak = rr / denominator;

    // xk += ak pk
    vector_axpy_inplace(xk, ak, pk);

    // rk -= ak A pk
    vector_axpy_inplace(rk, -ak, Apk);

    // rk_next^T rk_next
    double rr_next = dot(rk, rk);
    if (sqrt(rr_next) < TOL) {
      printf("Method converged at %d iterations.\n", k);
      free_vector(pk);
      free_vector(rk);
      free_vector(Apk);
      return xk;
    }

    // rk_next^T rk_next / rk^T rk
    double bk = rr_next / rr;

    // pk *= bk
    vector_scalar_product_inplace(pk, bk);

    // pk += rk
    vector_sum_inplace(pk, rk);
    rr = rr_next;
    k++;
  }
  free_vector(rk);
  free_vector(pk);
  free_vector(xk);
  fprintf(stderr, "Method failed after %d iterations\n", k);
  return NULL;
}
