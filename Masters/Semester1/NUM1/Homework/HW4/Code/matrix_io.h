#pragma once
#include "matrix.h"

double *flat_double_array_from_txt(const char *, int *);
int *int_array_from_txt(const char *, int *);
float *flat_float_array_from_txt(const char *, int *);
Matrix *load_matrix_from_txt(const char*, int*, int*);

double **load_non_contiguous_matrix_from_txt(const char *, int *, int *);
