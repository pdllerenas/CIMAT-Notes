#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_vector_dim(FILE *file, int *dim) {
  int c;
  *dim = 0;

  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      *dim += 1;
    }
  }
  rewind(file);
}

void get_matrix_dims(FILE *file, int *cols, int *rows) {
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
  int c, row_cnt = 1;
  get_matrix_dims(file, cols, rows);
  char line[1024];
  double **matrix = malloc(*rows * sizeof(double *));
  if (matrix == NULL) {
    fprintf(stderr, "Could not allocate memory for matrix.\n");
    fclose(file);
    exit(1);
  }
  int row_index, col_index;
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
      exit(1);
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

double *file_to_vector(FILE *file, int *dim) {
  *dim = 0;
  get_vector_dim(file, dim);

  char line[64];
  double *vec = malloc(*dim * sizeof(double *));
  if (vec == NULL) {
    fprintf(stderr, "Could not allocate memory for vector.\n");
    fclose(file);
    exit(1);
  }
  int row_index = 0;
  while (fgets(line, sizeof line, file) && row_index < *dim) {
    vec[row_index] = atof(line);
    row_index++;
  }
  return vec;
}

double dot(double *x, double *y, int dim) {
  double sum = 0;
  for (int i = 0; i < dim; i++) {
    sum += *(x + i) * *(y+i);
  }
  return sum;
}

double *ls_diagonal(double *D, double *b, int dim) {
  double *x = malloc(dim * sizeof(double));
  for (int i = 0; i < dim; i++) {
    *(x + i) = *(b + i) / *(D + i);
  }
  return x;
}

double *ls_lower(double **L, double *b, int dim) {
  double *x = calloc(dim, sizeof(double));
  *x = *b / **L;
  for (int i = 1; i < dim; i++) {
    *(x + i) = (*(b + i) - dot(x, *(L + i), i)) / L[i][i];
  }
  return x;
}

int main(int argc, char *argv[]) {
  if (argc > 4 || argc < 3) {
    fprintf(stderr,
            "Invalid number of arguments.\nUsage: %s <matrix-file> "
            "<vector-file> [D | U | L]\n",
            argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror(argv[1]);
    return 1;
  }

  double **matrix = NULL;
  double *diag_matrix = NULL;
  int cols, rows;

  if (argc == 4) {
    printf("chosen: %c\n", *argv[3]);
    switch (*argv[3]) {
    case 'D':
      printf("Creating diagonal matrix...\n");
      diag_matrix = create_diagonal_from_file(file, &cols, &rows);
      break;
    default:
      printf("Creating traingular matrix...\n");
      matrix = create_array_from_file(file, &cols, &rows);
    }
  } else {
    printf("Creating matrix...\n");
    matrix = create_array_from_file(file, &cols, &rows);
  }
  FILE *vector_b = fopen(argv[2], "r");
  if (!vector_b) {
    perror(argv[2]);
    return 1;
  }
  int v_dim;
  printf("Creating vector...\n");
  double *b = file_to_vector(vector_b, &v_dim);

  if (diag_matrix) {
    printf("Finding solution vector...\n");
    double *x = ls_diagonal(diag_matrix, b, rows);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (i == j) {
          printf("%lf ", diag_matrix[i]);
        } else {
          printf("0.000000 ");
        }
      }
      printf("\t%lf", x[i]);
      printf("\t\t%lf", b[i]);
      printf("\n");
    }

    free(diag_matrix);
    free(b);
    free(x);
  } else {
    printf("Finding solution vector...\n");
    double *x = ls_lower(matrix, b, rows);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        printf("%lf ", matrix[i][j]);
      }
      printf("\t%lf", x[i]);
      // printf("\t\t%lf", b[i]);
      printf("\n");
    }
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
    free(b);
    free(x);
  }
}
