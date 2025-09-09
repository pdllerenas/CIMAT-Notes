#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_dim(FILE *file) {
  int c, dim;
  while ((c = fgetc(file)) != EOF) {
    printf("%c\n", c);
    if (c == '\n') {
      return dim;
    }
    if (c == ' ') {
      dim++;
    }
  }
  rewind(file);
  return 0;
}

double **create_array_from_file(FILE *file) {
  int c, dim = get_dim(file), row_cnt = 0;
  char line[1024];
  double **matrix = malloc(dim * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    exit(1);
  }
  int row_index = 0, col_index = 0;
  while (fgets(line, sizeof line, file) && row_index < dim) {
    matrix[row_index] = malloc(dim * sizeof(double));
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
    while (token != NULL && col_index < dim) {
      matrix[row_index][col_index] = atof(token);
      token = strtok(NULL, " ");
      col_index++;
    }
    row_index++;
  }
  printf("%d, %d\n", row_index, col_index);
  fclose(file);
  return matrix;
}

double *create_diagonal_from_file(FILE *file) {}

double *ls_diagonal(double *D, double *b, int dim) {
  double *x = malloc(dim * sizeof(double));
  for (int i = 0; i < dim; i++) {
    *(x + i) = *(b + i) / *(D + i);
  }
  return x;
}

int main(int argc, char *argv[]) {
  if (argc > 3 || argc < 2) {
    fprintf(stderr,
            "Invalid number of arguments.\nUsage: %s <file> [D | U | L]\n",
            argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror(argv[1]);
    return 1;
  }

  double **matrix;
  double *diag_matrix;
  int dim = 15;

  if (argc == 3) {
    switch (atoi(argv[2])) {
    case 'D':
      printf("Creating matrix...\n");
      diag_matrix = create_diagonal_from_file(file);
    default:
      printf("Creating matrix...\n");
      matrix = create_array_from_file(file);
    }
  } else {
    printf("Creating matrix...\n");
    matrix = create_array_from_file(file);
  }

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      printf("%lf ", matrix[i][j]);
    }
    printf("\n");
  }

  for (int i = 0; i < dim; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
