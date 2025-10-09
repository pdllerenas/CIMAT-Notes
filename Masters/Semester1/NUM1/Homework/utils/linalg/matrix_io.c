#include "matrix_io.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFINE_READER_FUNCTION(FUNC_NAME, TYPE, FORMAT_SPECIFIER)              \
  TYPE *FUNC_NAME(const char *filename, int *data_count, int *rows,            \
                  int *cols) {                                                 \
    FILE *file = fopen(filename, "r");                                         \
    if (!file) {                                                               \
      perror(filename);                                                        \
      return NULL;                                                             \
    }                                                                          \
    TYPE *array, cell;                                                         \
    int capacity = 8;                                                          \
    array = malloc(capacity * sizeof(TYPE));                                   \
    if (!array) {                                                              \
      perror("Could not allocate memory for matrix.\n");                       \
      fclose(file);                                                            \
      return NULL;                                                             \
    }                                                                          \
    *cols = 0;                                                                 \
    char c;                                                                    \
    while ((c = fgetc(file)) != '\n') {                                        \
      if (c == EOF) {                                                          \
        break;                                                                 \
      } else if (c == ' ') {                                                   \
        (*cols)++;                                                             \
      }                                                                        \
    }                                                                          \
    (*cols)++;                                                                 \
    rewind(file);                                                              \
    *data_count = 0;                                                           \
    while (fscanf(file, FORMAT_SPECIFIER, &cell) == 1) {                       \
      if (*data_count == capacity) {                                           \
        capacity *= 2;                                                         \
        TYPE *temp = realloc(array, capacity * sizeof(TYPE));                  \
        if (!temp) {                                                           \
          perror("Failed to reallocate memory.\n");                            \
          free(array);                                                         \
          fclose(file);                                                        \
          return NULL;                                                         \
        }                                                                      \
        array = temp;                                                          \
      }                                                                        \
      array[*data_count] = cell;                                               \
      (*data_count)++;                                                         \
    }                                                                          \
    if (*data_count < capacity) {                                              \
      TYPE *temp = realloc(array, *data_count * sizeof(TYPE));                 \
      if (!temp) {                                                             \
        perror("Failed to downsize array.\n");                                 \
        free(array);                                                           \
        fclose(file);                                                          \
        return NULL;                                                           \
      }                                                                        \
      array = temp;                                                            \
    }                                                                          \
    *rows = *data_count / *cols;                                               \
    fclose(file);                                                              \
    return array;                                                              \
  }

DEFINE_READER_FUNCTION(flat_double_array_from_txt, double, "%lf")
DEFINE_READER_FUNCTION(int_array_from_txt, int, "%d")
DEFINE_READER_FUNCTION(float_array_from_txt, float, "%f")

/*

creates a Matrix object with its dimensions and data. Copies the contents of the
read filename into the matrix data attribute

 */
Matrix *load_matrix_from_txt(const char *filename, int *rows, int *cols) {
  int count = 0;
  double *flat_data = flat_double_array_from_txt(filename, &count, rows, cols);
  if (!flat_data) {
    return NULL;
  }
  printf("%d, %d\n", *rows, *cols);

  if (count != (*rows) * (*cols)) {
    fprintf(stderr,
            "Error: Data count in file does not match matrix dimensions.\n");
    free(flat_data);
    return NULL;
  }

  Matrix *matrix = matrix_create_double(*rows, *cols);
  if (!matrix) {
    free(flat_data);
    return NULL;
  }

  double *matrix_data = matrix->data;
  int k = 0;
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->cols; j++) {
      int offset_column = j % matrix->rows;
      matrix_data[i * *cols + offset_column] = flat_data[k++];
    }
  }
  matrix->data = matrix_data;
  free(flat_data);
  return matrix;
}

/*

generates a nx2 matrix containing the coefficients to the heat equation in 1
dimension

 */

Matrix *create_heat_matrix_1d(int dim) {
  Matrix *m = matrix_create_double(dim, 2);
  if (!m) {
    return NULL;
  }
  for (int i = 0; i < dim; i++) {
    ((double *)m->data)[2 * i] = 2;
    ((double *)m->data)[2 * i + 1] = -1;
  }
  return m;
}

void print_matrix(const Matrix *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->cols; j++) {
      printf("%lf ", ((double *)A->data)[i * A->cols + j]);
    }
    printf("\n");
  }
}
