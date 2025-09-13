#include <stdio.h>
#include <stdlib.h>

#include "p1.h"
#include "p2.h"
#include "vectors.h"

double *gaussian_elimination(double **A, double *b, int dim) {
  for (int i = 0; i < dim - 1; i++) {
    for (int p = i; p < dim; p++) {
      // if a non-zero entry is found but it is not in the diagonal,
      // swap the m_rows and break
      // (we are looking for the smallest p that satisfies this)
      if (A[p][i] && p != i) {
        double *temp = A[p];
        A[p] = A[i];
        A[i] = temp;
        break;
      } else if (A[p][i]) { // if it is already in the diagonal, break
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
      double m = A[j][i] / A[i][i];
      for (int k = 0; k < dim; k++) {
        A[j][k] = A[j][k] - m * A[i][k];
      }
    }
  }
  for (int i = 0; i < dim; i++) {
    if (A[i][i] == 0) {
      fprintf(stderr, "Unique solution does not exist.\n");
      exit(1);
    }
  }
  double *x = solve_upper(A, b, dim);
  return x;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "Invalid arguments. Usage: %s <matrix-file> <vector-file>.\n",
            argv[0]);
  }
  int m_cols = 0, m_rows = 0, v_dim;

  double **A = create_array_from_file(argv[1], &m_cols, &m_rows);
  if (!A) {
    exit(1);
  }
  double *b = file_to_vector(argv[2], &v_dim);
  if (!b) {
    for (int i = 0; i < m_rows; i++) {
      free(A[i]);
    }
    free(A);
  }

  if (m_cols != v_dim) {
    fprintf(stderr, "Matrix and vector dimensions do not match: %d and %d\n",
            m_cols, v_dim);
    exit(1);
  }

  gaussian_elimination(A, b, m_cols);
  double *x = solve_upper(A, b, m_rows);
  print_upper_solution(A, b, x, m_rows, m_cols);

  for (int i = 0; i < m_rows; i++) {
    free(A[i]);
  }
  free(A);
  return 0;
}
