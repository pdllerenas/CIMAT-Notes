#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
        L[i][j] = (1.0 / L[i][j]*(A[i][j] - sum));
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
			for(k = 0; k < j; k++) {
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

void crout(double const **A, double **L, double **U, int n) {
	int i, j, k;
	double sum = 0;

	for (i = 0; i < n; i++) {
		U[i][i] = 1;
	}

	for (j = 0; j < n; j++) {
		for (i = j; i < n; i++) {
			sum = 0;
			for (k = 0; k < j; k++) {
				sum = sum + L[i][k] * U[k][j];	
			}
			L[i][j] = A[i][j] - sum;
		}

		for (i = j; i < n; i++) {
			sum = 0;
			for(k = 0; k < j; k++) {
				sum = sum + L[j][k] * U[k][i];
			}
			if (L[j][j] == 0) {
				fprintf(stderr, "Can't divide by 0.\n");
				exit(EXIT_FAILURE);
			}
			U[j][i] = (A[j][i] - sum) / L[j][j];
		}
	}
}
