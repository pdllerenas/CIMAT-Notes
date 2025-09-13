#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

Reads the file and creates an array of doubles. We assume the file is of
upper triangular form; that the non-upper triangular values are all zero. Thus,
we ignore all other values and set them to zero automatically. Memory must be
free'd after called.

 */

double **create_upper_from_file(const char *filename, int *cols, int *rows) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }
  *rows = *cols = 0;
  int c, row_cnt = 0;
  get_matrix_dims(file, cols, rows);
  char line[1024];
  double **matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    return NULL;
  }
  int row_index = 0, col_index = 0;
  while (fgets(line, sizeof line, file) && row_index < *rows) {
    matrix[row_index] = calloc(*cols, sizeof(double));
    if (matrix[row_index] == NULL) {
      fprintf(stderr, "Could not allocate memory for matrix row %d.\n",
              row_index);
      for (int i = 0; i < *rows - row_index; i++) {
        free(matrix[i]);
      }
      free(matrix);
      fclose(file);
      return NULL;
    }
    col_index = 0;
    char *token = strtok(line, " ");
    while (token != NULL && col_index < *cols) {
      if (row_index <= col_index) {
        matrix[row_index][col_index] = atof(token);
      }
      token = strtok(NULL, " ");
      col_index++;
    }
    row_index++;
    row_cnt++;
  }

  fclose(file);
  return matrix;
}

/*

Solve the linear system Ux = b, assuming U is an upper triangular matrix.

 */

double *solve_upper(double **U, double *b, int dim) {
  double *x = malloc(dim * sizeof(double));
  if (U[dim - 1][dim - 1] == 0) {
    fprintf(stderr, "Unique solution does not exist.\n");
    return NULL;
  }
  x[dim - 1] = b[dim - 1] / U[dim - 1][dim - 1];
  for (int i = dim - 2; i >= 0; i--) {
    if (U[i][i] == 0) {
      fprintf(stderr, "Unique solution does not exist.\n");
      return NULL;
    }
    double sum = dot(x + i + 1, &U[i][i + 1], dim - i - 1);
    x[i] = (b[i] -sum) / U[i][i];
  }
  return x;
}

void print_upper_solution(double **U, double *b, double *x, int rows,
                          int cols) {
  printf("= = = = = = = = = = = = = = = = = = = = MATRIX U = = = = = = = = = = "
         "= = = = = = = = = = =  = x =      = b =\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%05.2lf ", U[i][j]);
    }
    printf("| %06.2lf | ", x[i]);
    printf(" | %06.2lf | ", b[i]);
    printf("\n");
  }
  printf("====================================================================="
         "========================================\n");
}
