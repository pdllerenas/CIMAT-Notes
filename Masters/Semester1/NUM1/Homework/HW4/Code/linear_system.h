#pragma once

#include "matrix.h"
#include "vector.h"

Vector *solve_diagonal(Matrix *, Vector *);
Vector *solve_upper(Matrix *, Vector *);
Vector *solve_lower(Matrix *, Vector *);
Vector *jacobi_iterative(Matrix *, Vector *, Vector *, double, int);
Vector *gauss_seidel_iterative(Matrix *, Vector *, Vector *, double, int);
