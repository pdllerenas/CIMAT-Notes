#include "matrix_io.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFINE_READER_FUNCTION(FUNC_NAME, TYPE, FORMAT_SPECIFIER)              \
  TYPE *FUNC_NAME(const char *filename, int *data_count) {                     \
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
    fclose(file);                                                              \
    return array;                                                              \
  }

DEFINE_READER_FUNCTION(flat_double_array_from_txt, double, "%lf");
DEFINE_READER_FUNCTION(int_array_from_txt, int, "%d");
DEFINE_READER_FUNCTION(float_array_from_txt, float, "%f");

double **load_non_contiguous_matrix_from_txt(const char *filename, int *rows,
                                             int *cols) {
  int count = 0;
  double *flat_data = flat_double_array_from_txt(filename, &count);
  if (!flat_data) {
    return NULL;
  }
  if (count != (*rows) * (*cols)) {
    fprintf(stderr,
            "Error: Data count in file does not match matrix dimensions.\n");
    free(flat_data);
    return NULL;
  }

  double **matrix =
      matrix_create_double_non_contiguous(*rows, *cols, MATRIX_FULL);
  if (!matrix) {
    free(flat_data);
    return NULL;
  }

  for (int i = 0; i < *rows; i++) {
    for (int j = 0; j < *cols; j++) {
      matrix[i][j] = flat_data[i * (*cols) + j];
    }
  }
  free(flat_data);
  return matrix;
}

Matrix *load_matrix_from_txt(const char *filename, int *rows, int *cols) {
  int count = 0;
  double *flat_data = flat_double_array_from_txt(filename, &count);
  if (!flat_data) {
    return NULL;
  }
  if (count != (*rows) * (*cols)) {
    fprintf(stderr,
            "Error: Data count in file does not match matrix dimensions.\n");
    free(flat_data);
  }

  Matrix *matrix = matrix_create_double(*rows, *cols);
  if (!matrix) {
    free(flat_data);
    return NULL;
  }

  double **matrix_data = matrix->data;
  int k = 0;
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->cols; j++) {
      int offset_column = j % matrix->rows;
      matrix_data[i][offset_column] = flat_data[k++];
    }
  }
  free(flat_data);
  return matrix;
}
