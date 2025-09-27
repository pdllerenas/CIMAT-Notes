#pragma once

#include "matrix.h"
#include "vector.h"

Matrix *matrix_product(const Matrix *, const Matrix *);
Matrix *matrix_transpose(const Matrix *);
Matrix *matrix_difference(const Matrix *, const Matrix *);
Matrix *matrix_sum(const Matrix *, const Matrix *);
int matrix_compare(const Matrix *, const Matrix *, double);
void matrix_swap_rows(Matrix *, const int, const int);
Vector *matrix_times_vector(const Matrix *, const Vector *);
Matrix *deflation_term(const Vector *, double);
