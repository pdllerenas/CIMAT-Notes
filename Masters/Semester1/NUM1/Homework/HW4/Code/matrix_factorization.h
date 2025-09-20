#pragma once
#include "matrix.h"

void crout(const Matrix *A, Matrix *L, Matrix *U, int n);
void doolittle(double const **A, double **L, double **U, int n);
void cholesky(double const **A, double **L, int n);

