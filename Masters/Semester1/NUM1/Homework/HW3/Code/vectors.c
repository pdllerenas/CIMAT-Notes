#include <stdlib.h>
#include "vectors.h"

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
    sum += *(x + i) * *(y + i);
  }
  return sum;
}
