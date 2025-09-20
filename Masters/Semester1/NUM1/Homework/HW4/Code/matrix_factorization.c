#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "matrix_factorization.h"

void cholesky(double const **A, double **L, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) {
      double sum = 0;
      for (int k = 0; k < j; k++) {
        sum += L[i][k] * L[j][k];
      }

      if (i == j) {
        L[i][j] = sqrt(A[i][j] - sum);
      } else {
        L[i][j] = (1.0 / L[i][j] * (A[i][j] - sum));
      }
    }
  }
}

void doolittle(double const **A, double **L, double **U, int n) {
  int i, j, k;
  double sum = 0;

  for (i = 0; i < n; i++) {
    L[i][i] = 1;
  }

  for (j = 0; j < n; j++) {
    for (i = j; i < n; i++) {
      sum = 0;
      for (k = 0; k < j; k++) {
        sum += U[i][k] * L[k][j];
      }
      U[i][j] = A[i][j] - sum;
    }

    for (i = j; i < n; i++) {
      sum = 0;
      for (k = 0; k < j; k++) {
        sum = sum + U[j][k] * L[k][i];
      }
      if (U[j][j] == 0) {
        fprintf(stderr, "Can't divide by 0.\n");
        exit(EXIT_FAILURE);
      }
      L[j][i] = (A[j][i] - sum) / U[j][j];
    }
  }
}

void crout(const Matrix *A, Matrix *L, Matrix *U, int n) {
  int i, j, k;
  double sum = 0;

  for (i = 0; i < n; i++) {
    ((double *)U->data)[i * n + i] = 1;
  }

  for (j = 0; j < n; j++) {
    for (i = j; i < n; i++) {
      sum = 0;
      for (k = 0; k < j; k++) {
        sum = sum +
              ((double *)L->data)[i * n + k] * ((double *)U->data)[k * n + j];
      }
      ((double *)L->data)[i * n + j] = ((double *)A->data)[i * n + j] - sum;
    }

    for (i = j; i < n; i++) {
      sum = 0;
      for (k = 0; k < j; k++) {
        sum = sum +
              ((double *)L->data)[j * n + k] * ((double *)U->data)[k * n + i];
      }
      if (((double *)L->data)[j * n + j] == 0) {
        fprintf(stderr, "Can't divide by 0.\n");
        exit(EXIT_FAILURE);
      }
      ((double *)U->data)[j * n + i] = (((double *)A->data)[j * n + i] - sum) /
                                       ((double *)L->data)[j * n + j];
    }
  }
}
