#pragma once
#include "matrix.h"

void crout(const Matrix *A, Matrix *L, Matrix *U, int n);
void doolittle(const Matrix *A, Matrix *L, Matrix *U, int n);
void cholesky(const Matrix *A, Matrix *L, int n);
void cholesky_symmetric_banded(const Matrix *A, Matrix *L);
void householder(Matrix *A);
