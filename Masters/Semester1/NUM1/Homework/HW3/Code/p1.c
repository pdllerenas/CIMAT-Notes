#include "p1.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double **create_array_from_file(FILE *file, int *cols, int *rows) {
  *rows = *cols = 0;
  int row_cnt;
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
    matrix[row_index] = malloc(*cols * sizeof(double));
    if (matrix[row_index] == NULL) {
      fprintf(stderr, "Could not allocate memory for matrix row %d.\n",
              row_index);
      for (int i = 0; i < row_index; i++) {
        free(matrix[i]);
      }
      free(matrix);
      fclose(file);
      exit(1);
    }
    col_index = 0;
    char *token = strtok(line, " ");
    while (token != NULL && col_index < *cols) {
      matrix[row_index][col_index] = atof(token);
      token = strtok(NULL, " ");
      col_index++;
    }
    row_index++;
  }
  fclose(file);
  return matrix;
}

double *create_diagonal_from_file(FILE *file, int *cols, int *rows) {
  *rows = *cols = 0;
  get_matrix_dims(file, cols, rows);
  char line[1024];
  double *matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    exit(1);
  }
  int row_index = 0, col_index = 0;
  while (fgets(line, sizeof line, file) && row_index < *rows) {
    char *token = strtok(line, " ");
    while (token != NULL && row_index < *rows && col_index < *cols) {
      if (row_index == col_index) {
        matrix[row_index] = atof(token);
      }
      token = strtok(NULL, " ");
      col_index++;
    }
    col_index = 0;
    row_index++;
  }
  return matrix;
}

double *solve_diagonal(double *D, double *b, int dim) {
  double *x = malloc(dim * sizeof(double));
  for (int i = 0; i < dim; i++) {
    *(x + i) = *(b + i) / *(D + i);
  }
  return x;
}

void print_diagonal_solution(double rows, double cols, double* diag_matrix, double *x, double *b) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i == j) {
        printf("%lf ", diag_matrix[i]);
      } else {
        printf("%lf ", 0.0);
      }
    }
    printf("\t%lf", x[i]);
    printf("\t\t%lf", b[i]);
    printf("\n");
  }
}
