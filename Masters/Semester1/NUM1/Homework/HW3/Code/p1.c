#include "p1.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

Reads and parses the given file. We assume it uses space as a separator, and \n
as the end of line.
Return a double pointer of doubles. Memory must be free'd after call.

 */
double **create_array_from_file(const char *filename, int *cols, int *rows) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }
  *rows = *cols = 0;
  int row_cnt;
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
    matrix[row_index] = malloc(*cols * sizeof(double));
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

/*

Reads the file and creates an array of doubles. We assume the file is of
diagonal form; that the non-diagonal values are all zero. Thus, we ignore all
other values and set them to zero automatically. Memory must be free'd after
called.

 */

double *create_diagonal_from_file(const char *filename, int *cols, int *rows) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }
  *rows = *cols = 0;
  get_matrix_dims(file, cols, rows);
  char line[1024];
  double *matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    return NULL;
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

/*

Solve the system assuming a diagonal matrix. Memory must be free'd after call.

 */

double *solve_diagonal(double *D, double *b, int dim) {
  double *x = malloc(dim * sizeof(double));
  if (!x) {
    perror("Could not allocate memory for solution vector.\n");
    return NULL;
  }
  for (int i = 0; i < dim; i++) {
    x[i] = b[i] / D[i];
  }
  return x;
}

/*

We print the diagonal matrix, the solution vector, and the resulting vector.
Note that we assume that the input is a diagonal matrix, so we print all other
values as 0.

 */

void print_diagonal_solution(int rows, int cols, double *diag_matrix,
                             double *x, double *b) {
  printf("= = = = = = = = = = = = = = = = = = = = MATRIX D = = = = = = = = = = = = = = = = = = = = =  = x =      = b =\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i == j) {
        printf("%05.2lf ", diag_matrix[i]);
      } else {
        printf("%05.2lf ", 0.0);
      }
    }
    printf("| %05.2lf | ", x[i]);
    printf(" | %05.2lf |", b[i]);
    printf("\n");
  }
  printf("=============================================================================================================\n");
}
