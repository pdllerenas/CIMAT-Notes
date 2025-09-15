#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "p1.h"
#include "p2.h"
#include "vectors.h"

double *gaussian_pivoting(double **A, double *b, int dim) {
  int *col_indicator = malloc(dim * sizeof(int));
  if (!col_indicator) {
    perror("Could not allocate memory.\n");
    return NULL;
  }
  for (int i = 0; i < dim; i++) {
    col_indicator[i] = i;
  }
  double max_val, temp;
  int row_pivot, col_pivot;

  for (int i = 0; i < dim; i++) {
    max_val = 0;
    row_pivot = col_pivot = i;
    for (int p = i; p < dim; p++) {
      for (int j = i; j < dim; j++) {
        if (fabs(A[p][j]) > max_val) {
          max_val = fabs(A[p][j]);
          row_pivot = p;
          col_pivot = j;
        }
      }
      if (max_val == 0) {
        fprintf(stderr, "Unique solution not found.\n");
        free(col_indicator);
        return NULL;
      }
      if (row_pivot != i) {
        double *temp_row = A[i];
        A[i] = A[row_pivot];
        A[row_pivot] = temp_row;

        temp = b[i];
        b[i] = b[row_pivot];
        b[row_pivot] = temp;
      }

      if (col_pivot != i) {
        for (int m = 0; m < dim; m++) {
          temp = A[m][i];
          A[m][i] = A[m][col_pivot];
          A[m][col_pivot] = temp;
        }
        int temp_col = col_indicator[i];
        col_indicator[i] = col_indicator[col_pivot];
        col_indicator[col_pivot] = temp_col;
      }
    }
    for (int j = i + 1; j < dim; j++) {
      double factor = A[j][i] / A[i][i];
      for (int k = i + 1; k < dim; k++) {
        A[j][k] = A[j][k] - factor * A[i][k];
      }
      b[j] = b[j] - factor * b[i];
    }
  }
  double *x = solve_upper(A, b, dim);
  free(col_indicator);
  return x;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "Invalid arguments. Usage: %s <matrix-file> <vector-file>.\n",
            argv[0]);
  }
  int m_cols, m_rows, v_dim;

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
    exit(1);
  }

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
