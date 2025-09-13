#include <stdio.h>

void get_vector_dim(FILE *file, int *dim);
void get_matrix_dims(FILE *file, int *cols, int *rows);
double *file_to_vector(const char* filename, int *dim);
double dot(double *x, double *y, int dim);
