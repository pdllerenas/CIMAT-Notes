#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "p1.h"
#include "p2.h"
#include "vectors.h"

double *gaussian_pivoting(double **A, double *b, int dim) {
  int *row_indicator = malloc(dim * sizeof(int));
  for (int i = 0; i < dim; i++) {
    row_indicator[i] = i;
  }
  double pivot = 0;

  for (int i = 0; i < dim - 1; i++) {
    for (int p = i; p < dim; p++) {
      // if a non-zero entry is found but it is not in the diagonal,
      // swap the rows and break
      // (we are looking for the smallest p that satisfies this)
      if (fabs(A[row_indicator[p]][i]) > fabs(pivot) && row_indicator[p] != row_indicator[i]) {
        int temp = row_indicator[i];
        row_indicator[i] = row_indicator[p];
        row_indicator[p] = temp;
        break;
      } else if (A[p][i]) { // if it is already in the diagonal, break
        break;
      }
      // if we have arrived to the final p and no non-zero A[p][i] was found,
      // may conclude that the system has no unique solution.
      else if (p == dim - 1) {
        fprintf(stderr, "Unique solution does not exist.\n");
        return NULL;
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
  FILE *matrix_file = fopen(argv[1], "r");
  if (!matrix_file) {
    perror(argv[1]);
    exit(1);
  }
  FILE *vector_file = fopen(argv[2], "r");
  if (!vector_file) {
    perror(argv[1]);
    exit(1);
  }

  int m_cols, m_rows, v_dim;

  double **A = create_array_from_file(matrix_file, &m_cols, &m_rows);
  double *b = file_to_vector(vector_file, &v_dim);

  if (m_cols != v_dim) {
    fprintf(stderr, "Matrix and vector dimensions do not match: %d and %d\n",
            m_cols, v_dim);
    exit(1);
  }

  gaussian_pivoting(A, b, m_cols);
  double *x = solve_upper(A, b, m_rows);
  print_upper_solution(A, b, x, m_rows, m_cols);

  for (int i = 0; i < m_rows; i++) {
    free(A[i]);
  }
  free(A);
  return 0;
}
