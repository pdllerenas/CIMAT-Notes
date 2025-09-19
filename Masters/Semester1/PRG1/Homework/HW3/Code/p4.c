#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*

calculate product of all N MxM matrices

 */

double **multiply_3d_matrix(double ***pmatrix, int M, int N) {
  double **result = malloc(M * sizeof(double *));

  // for storing result * next matrix and not alter result in the process
  double **temp = malloc(M * sizeof(double *));

  for (int i = 0; i < M; i++) {
    result[i] = malloc(M * sizeof(double));
    temp[i] = malloc(M * sizeof(double));
  }

  // 0th iteration of multiplication is simply our first matrix
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      result[i][j] = pmatrix[i][j][0];
    }
  }

  // begin iterative process for other matrices
  for (int n = 1; n < N; n++) {     // matrix index
    for (int i = 0; i < M; i++) {   // row
      for (int j = 0; j < M; j++) { // col
        double sum = 0.0;
        for (int k = 0; k < M; k++) {
          sum += result[i][k] * pmatrix[k][j][n];
        }
        temp[i][j] = sum;
      }
    }
    // swap pointers to update result with temp
    double **swap = result;
    result = temp;
    temp = swap;
  }

  // Free the memory for the temporary matrix
  for (int i = 0; i < M; i++) {
    free(temp[i]);
  }
  free(temp);

  return result;
}

/*

calculate sum of all matrices, entry-wise

 */

double **sum_3d_matrix(double ***pmatrix, int M, int N) {
  double **result = malloc(M * sizeof(double));
  double sum = 0;

  for (int i = 0; i < M; i++) {
    result[i] = malloc(M * sizeof(double));
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < N; k++) {
        sum += pmatrix[i][j][k]; // sum all i,j in a single loop
      }
      result[i][j] = sum;
      sum = 0;
    }
  }
  return result;
}

/*

calculate first matrix minus the rest

 */

double **difference_3d_matrix(double ***pmatrix, int M, int N) {
  double **result = malloc(M * sizeof(double));
  double sum;

  for (int i = 0; i < M; i++) {
    result[i] = malloc(M * sizeof(double));
    for (int j = 0; j < M; j++) {
      sum = pmatrix[i][j][0];
      for (int k = 0; k < N; k++) {
        sum -= pmatrix[i][j][k];
      }
      result[i][j] = sum;
    }
  }
  return result;
}

/*

generate N MxM matrices and store them in .bin files,
returns their filenames

 */

char **create_and_get_filenames(const int M, const int N) {
  srand(time(NULL));
  const int rows = M;
  const int cols = M;

  char **filenames = malloc(N * sizeof(char *));
  if (filenames == NULL) {
    perror("Memory allocation failed.\n");
    return NULL;
  }

  for (int i = 0; i < N; ++i) {
    filenames[i] = malloc(20 * sizeof(char));
    if (filenames[i] == NULL) {
      perror("Memory allocation failed.\n");
      for (int j = 0; j < i; ++j) {
        free(filenames[j]);
      }
      free(filenames);
      return NULL;
    }

    snprintf(filenames[i], 22, "matrix_%d.bin", i + 1);

    FILE *file = fopen(filenames[i], "wb");
    if (file == NULL) {
      perror("Error creating file.\n");
      continue;
    }

    double matrix[rows][cols];
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        matrix[r][c] = (double)((rand() % 10) * (rand() % 10));
      }
    }

    size_t written = fwrite(matrix, sizeof(double), rows * cols, file);
    if (written != rows * cols) {
      fprintf(stderr, "Error writing to file %s\n", filenames[i]);
    }

    fclose(file);
  }
  return filenames;
}

/*

given a list of N filenames (binary files) of MxM matrices,
returns a triple pointer to a double, which is each MxM matrix
in layers

 */

double ***create_3d_matrix_from_file(char *filename[], int M, int N) {
  FILE **file = malloc(N * sizeof(FILE *));
  for (int i = 0; i < N; i++) {
    file[i] = fopen(filename[i], "rb");
    if (!file[i]) {
      perror(filename[i]);
      return NULL;
    }
  }
  double ***pmatrix = malloc(M * sizeof(double *));
  for (int i = 0; i < M; i++) {
    pmatrix[i] = malloc(M * sizeof(double *));
    for (int j = 0; j < M; j++) {
      pmatrix[i][j] = malloc(N * sizeof(double));
      for (int k = 0; k < N; k++) {
        const size_t ret_code =
            fread(&pmatrix[i][j][k], sizeof(double), 1, file[k]);
        if (ret_code != 1) {
          if (feof(file[i])) {
            fprintf(stderr, "Error reading %s: unexpected end of file.\n",
                    filename[i]);
          } else if (ferror(file[i])) {
            perror(filename[i]);
          }
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    fclose(file[i]);
  }

  free(file);
  return pmatrix;
}

/*

Frees the 3d matrix

 */

void free_3d_marix(double ***pmatrix, int M, int N) {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      free(pmatrix[i][j]);
    }
    free(pmatrix[i]);
  }
  free(pmatrix);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "Invalid argument count. Usage: %s <matrix dimension> <number of "
            "matrices>",
            argv[0]);
    exit(1);
  }
  int N = atoi(argv[2]);
  int M = atoi(argv[1]);

  char **filenames = create_and_get_filenames(M, N);
  double ***pmatrix = create_3d_matrix_from_file(filenames, M, N);

  double **product_matrix = multiply_3d_matrix(pmatrix, M, N);
  double **sum_matrix = sum_3d_matrix(pmatrix, M, N);
  double **diff_matrix = difference_3d_matrix(pmatrix, M, N);

  if (filenames != NULL) {
    for (int i = 0; i < N; ++i) {
      if (filenames[i] != NULL) {
        free(filenames[i]);
      }
    }
    free(filenames);
  }

  for (int k = 0; k < N; k++) {
    printf(" matrix %d:\n", k);
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        printf("%lf ", pmatrix[i][j][k]);
      }
      printf("\n");
    }
    printf("\n");
  }

  printf("product matrix:\n");
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      printf("%lf ", product_matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  printf("sum matrix:\n");
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      printf("%lf ", sum_matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  printf("diff matrix:\n");
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      printf("%lf ", diff_matrix[i][j]);
    }
    printf("\n");
  }
  free_3d_marix(pmatrix, M, N);
  for (int i = 0; i < M; i++) {
    free(product_matrix[i]);
    free(sum_matrix[i]);
    free(diff_matrix[i]);
  }
  free(product_matrix);
  free(sum_matrix);
  free(diff_matrix);
}
