#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

The following function assumes that the file passed into it has the form of a
triangular matrix. That is, only the lower portion of the file will be read,
while all others will be ignored, assuming they are zero.

 */

double **create_lower_from_file(const char* filename, int *cols, int *rows) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }
  *rows = *cols = 0;
  int c, row_cnt = 1;
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
      for (int i = 0; i < row_index; i++) {
        free(matrix[i]);
      }
      free(matrix);
      fclose(file);
      return NULL;
    }
    col_index = 0;
    char *token = strtok(line, " ");
    while (token != NULL && col_index < row_cnt) {
      matrix[row_index][col_index] = atof(token);
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

Assuming that L is a lower triangular matrix,
we solve for the linear system Lx = b.

 */

double *solve_lower(double **L, double *b, int dim) {
  double *x = malloc(dim * sizeof(double));
  if (!x) {
    perror("Could not allocate memory for solution vector.\n");
    return NULL;
  }
  if (**L == 0) {
    fprintf(stderr, "System has no unique solution.\n");
    return NULL;
  }
  *x = *b / **L; // first element of solution
  for (int i = 1; i < dim; i++) {
    if (L[i][i] == 0) {
      fprintf(stderr, "System has no unique solution.\n");
      return NULL;
    }
    double sum = dot(x, L[i], i);
    x[i] = (b[i] - sum) / L[i][i];
  }
  return x;
}

void print_lower_solution(double **L, double *x, double *b, int rows,
                          int cols) {
  printf("= = = = = = = = = = = = = = = = = = = = MATRIX L = = = = = = = = = = "
         "= = = = = = = = = = =  = x =      = b =\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%05.2lf ", L[i][j]);
    }
    printf("| %05.2lf |", x[i]);
    printf(" | %05.2lf |", b[i]);
    printf("\n");
  }
}
