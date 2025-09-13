#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>

/*

reads the file and modifies dim to the number of new lines found.

 */
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

/*

reads the matrix file and edits the cols and rows parameters passed. We assume
space as the separator, \n as the newline.

 */

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

/*

reads and parses the file, returning a pointer. We assume the endline char is
\n.

 */

double *file_to_vector(const char *filename, int *dim) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }
  *dim = 0;
  get_vector_dim(file, dim);

  char line[64];
  double *vec = malloc(*dim * sizeof(double *));
  if (vec == NULL) {
    fprintf(stderr, "Could not allocate memory for vector.\n");
    fclose(file);
    return NULL;
  }
  int row_index = 0;
  while (fgets(line, sizeof line, file) && row_index < *dim) {
    vec[row_index] = atof(line);
    row_index++;
  }
  fclose(file);
  return vec;
}

/*

dot product of x and y. We iterate only dim positions for both x and y. We also
assume x and y have enough memory allocated for it to iterate through them.

 */

double dot(double *x, double *y, int dim) {
  double sum = 0;

  for (int i = 0; i < dim; i++) {
    sum += *(x + i) * *(y + i);
  }
  return sum;
}
