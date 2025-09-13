#include "vectors.h"
#include <stdlib.h>
#include <string.h>

double **create_upper_from_file(FILE *file, int *cols, int *rows) {
  *rows = *cols = 0;
  int c, row_cnt = 0;
  get_matrix_dims(file, cols, rows);
  char line[1024];
  double **matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    exit(1);
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
      exit(1);
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

double *solve_upper(double **U, double *b, int dim) {
  double *x = calloc(dim, sizeof(double));
  *x = *(b + dim - 1);
  for (int i = dim - 1; i >= 0; i--) {
    *(x + i) = (*(b + i) - dot(x, *(U + i), dim - i)) / U[i][i];
  }
  return x;
}

void print_upper_solution(double **U, double *b, double *x, int rows,
                          int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%lf\t", U[i][j]);
    }
    printf("\t\t%lf", x[i]);
    printf("=\t%lf", b[i]);
    printf("\n");
  }
}
