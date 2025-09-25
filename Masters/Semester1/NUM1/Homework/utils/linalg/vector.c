#include "vector.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*

initializes a vector object

 */

Vector *create_vector(int dim) {
  Vector *v = malloc(sizeof(Vector));
  v->dim = dim;
  v->type = sizeof(double);
  v->data = malloc(dim * sizeof(double));
  return v;
}

/*

returns the dot product of two vectors

 */

double dot(const Vector *v1, const Vector *v2) {
  if (v1->dim != v2->dim) {
    fprintf(stderr, "Vector dimension mismatch. %d does not equal %d.\n",
            v1->dim, v2->dim);
    exit(1);
  }
  double sum = 0.0;
  for (int i = 0; i < v1->dim; i++) {
    sum += ((double *)v1->data)[i] * ((double *)v2->data)[i];
  }
  return sum;
}

/*

compares v1 and v2, returns 0 if equal (up to a tolerance), -1 if not
comparable, 1 if comparable but different

 */

int vector_compare(const Vector *v1, const Vector *v2, double TOL) {
  if (v1->dim != v2->dim) {
    fprintf(stderr, "Vector dimension mismatch. %d does not equal %d.\n",
            v1->dim, v2->dim);
    return -1;
  }
  for (int i = 0; i < v1->dim; i++) {
    if (fabs(((double *)v1->data)[i] - ((double *)v2->data)[i]) > TOL) {
      return 1;
    }
  }

  return 0;
}

/*

returns the vector a+b

 */

Vector *vector_sum(const Vector *v1, const Vector *v2) {
  if (v1->dim != v2->dim) {
    fprintf(stderr, "Vector dimension mismatch. %d does not equal %d.\n",
            v1->dim, v2->dim);
    exit(1);
  }
  Vector *s = create_vector(v1->dim);
  for (int i = 0; i < v1->dim; i++) {
    ((double *)s->data)[i] = ((double *)v1->data)[i] + ((double *)v2->data)[i];
  }
  return s;
}

Vector *vector_diff(const Vector *v1, const Vector *v2) {
  if (v1->dim != v2->dim) {
    fprintf(stderr, "Vector dimension mismatch. %d does not equal %d.\n",
            v1->dim, v2->dim);
    exit(1);
  }
  Vector *s = create_vector(v1->dim);
  for (int i = 0; i < v1->dim; i++) {
    ((double *)s->data)[i] = ((double *)v1->data)[i] - ((double *)v2->data)[i];
  }
  return s;
}

double vector_norm_squared(const Vector *v) {
  double norm = 0.0;
  for (int i = 0; i < v->dim; i++) {
    norm += ((double *)v->data)[i] * ((double *)v->data)[i];
  }
  return norm;
}

/*

creates a vector object given a filename

 */

Vector *load_vector_from_txt(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }

  int size = 16, idx = 0;
  double cell;
  Vector *v = create_vector(size);

  while (fscanf(file, "%lf", &cell) == 1) {
    if (idx == size) {
      size *= 2;
      double *temp = realloc(v->data, size * sizeof(double));
      if (!temp) {
        perror("Could not reallocate memory.\n");
        free_vector(v);
        fclose(file);
        return NULL;
      }
      v->data = temp;
      v->dim = size;
    }
    ((double *)v->data)[idx++] = cell;
  }
  if (idx < size) {
    double *temp = realloc(v->data, idx * sizeof(double));
    if (!temp) {
      perror("Could not reallocate memory.\n");
    }
    v->data = temp;
    v->dim = idx;
  }
  fclose(file);
  return v;
}

/*

creates a vector filled with the value value

*/

Vector *vector_fill(int dim, double value) {
  Vector *v = create_vector(dim);

  for (int i = 0; i < dim; i++) {
    ((double *)v->data)[i] = value;
  }
  return v;
}

/*

copies the contents of vector v2 into vector v1

 */

void copy_data(Vector *v1, const Vector *v2) {
  for (int i = 0; i < v1->dim; i++) {
    ((double *)v1->data)[i] = ((double *)v2->data)[i];
  }
}

void print_vector(Vector *v) {
  for (int i = 0; i < v->dim; i++) {
    printf("%lf ", ((double *)v->data)[i]);
  }
  printf("\n");
}

void vector_swap_rows(Vector *v, const int i, const int j) {
  double temp = ((double *)v->data)[i];
  ((double *)v->data)[i] = ((double *)v->data)[j];
  ((double *)v->data)[j] = temp;
}

void free_vector(Vector *v) {
  free(v->data);
  free(v);
}

int vector_arg_max(Vector *v) {
  int arg_max = 0;
  double max_val = fabs(((double *)v->data)[arg_max]);
  for (int i = 1; i < v->dim; i++) {
    if (max_val < fabs(((double *)v->data)[i])) {
      max_val = fabs(((double *)v->data)[i]);
      arg_max = i;
    }
  }
  return arg_max;
}

void vector_scalar_product(Vector *v, const double k) {
  for (int i = 0; i < v->dim; i++) {
    ((double *)v->data)[i] *= k;
  }
}

double max_norm(const Vector *v) {
  double norm = ((double *)v->data)[0];
  for (int i = 1; i < v->dim; i++) {
    if (norm < ((double *)v->data)[i]) {
      norm = ((double *)v->data)[i];
    }
  }
  return norm;
}

double l2_norm(const Vector *v) {
  double norm = 0;
  for (int i = 0; i < v->dim; i++) {
    norm += ((double *)v->data)[i] * ((double *)v->data)[i];
  }
  return sqrt(norm);
}
