#pragma once

#include "matrix.h"
#include "vector.h"

Vector *solve_diagonal(Matrix *, Vector *);
Vector *solve_upper(Matrix *, Vector *);
Vector *solve_lower(Matrix *, Vector *);
Vector *jacobi_iterative(Matrix *, Vector *, Vector *, double, int);
Vector *gauss_seidel_iterative(Matrix *, Vector *, Vector *, double, int);
Vector *gaussian_elimination(Matrix *A, Vector *b);
Vector *cholesky_solve(Matrix *L, Vector *b);
Vector *solve_upper_transpose(Matrix *L, Vector *b);
Vector *LU_solve(Matrix *L, Matrix *U, Vector *b);
Vector *conjugate_gradient(Matrix *A, Vector *b, Vector *x0, double TOL,
                           int MAX_ITER);
