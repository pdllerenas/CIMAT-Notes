#pragma once
#include "matrix.h"

double *flat_double_array_from_txt(const char *, int *, int *, int *);
int *int_array_from_txt(const char *, int *, int *, int *);
float *flat_float_array_from_txt(const char *, int *, int *, int *);
Matrix *load_matrix_from_txt(const char*, int *, int *);
Matrix *create_heat_matrix_1d(int dim);
void print_matrix(Matrix *);

double **load_non_contiguous_matrix_from_txt(const char *, int *, int *);
