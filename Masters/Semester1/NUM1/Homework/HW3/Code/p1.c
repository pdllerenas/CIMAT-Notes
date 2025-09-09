#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_dim(FILE *file, int *cols, int *rows) {
  int c;
  *cols = 0;
  *rows = 0;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      *rows += 1;
    }
    if (c == ' ' && *rows == 0) {
      *cols += 1;
    }
  }
  rewind(file);
}

double **create_lower_from_file(FILE *file, int *cols, int *rows) {
  *rows = *cols = 0;
  int c, row_cnt;
  get_dim(file, cols, rows);
  char line[1024];
  double **matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    exit(1);
  }
  int row_index, col_index;
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
  printf("%d, %d\n", row_index, col_index);
  fclose(file);
  return matrix;
}


double **create_array_from_file(FILE *file, int *cols, int *rows) {
  *rows = *cols = 0;
  int c, row_cnt;
  get_dim(file, cols, rows);
  printf("%d, %d\n", *cols, *rows);
  char line[1024];
  double **matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    exit(1);
  }
  int row_index, col_index;
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
  printf("%d, %d\n", row_index, col_index);
  fclose(file);
  return matrix;
}

double *create_diagonal_from_file(FILE *file, int *cols, int *rows) {}

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
  int cols, rows;

  if (argc == 3) {
    switch (atoi(argv[2])) {
    case 'D':
      printf("Creating matrix...\n");
      diag_matrix = create_diagonal_from_file(file, &cols, &rows);
    default:
      printf("Creating matrix...\n");
      matrix = create_array_from_file(file, &cols, &rows);
    }
  } else {
    printf("Creating matrix...\n");
    matrix = create_array_from_file(file, &cols, &rows);
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%lf ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("Done printing.\n");

  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
