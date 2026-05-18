#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "conjugate_gradient.h"

// assuming square matrix and same length vector x0, b

double *conjugate_gradient_sparse(int *I, int *J, double *V, int nnz, int N, double *x0, double *b, double TOL, int MAX_ITER)
{
  double *diag = (double *)malloc(N * sizeof(double));
  for (int i = 0; i < N; i++)
    diag[i] = 1.0;

  for (int i = 0; i < nnz; i++)
  {
    if (I[i] == J[i])
    {
      diag[I[i]] = V[i];
    }
  }

  double *Ax0 = (double *)calloc(N, sizeof(double));

#pragma omp parallel for default(none) shared(I, J, V, x0, Ax0, nnz)
  for (int i = 0; i < nnz; i++)
  {
    int row = I[i];
    int col = J[i];
#pragma omp atomic
    Ax0[row] += V[i] * x0[col];
  }

  double *rk = (double *)calloc(N, sizeof(double));
#pragma omp parallel for default(none) shared(rk, b, Ax0, N)
  for (int i = 0; i < N; i++)
  {
    rk[i] = b[i] - Ax0[i];
  }
  free(Ax0);

  double *zk = (double *)calloc(N, sizeof(double));
#pragma omp parallel for default(none) shared(zk, rk, diag, N)
  for (int i = 0; i < N; i++)
  {
    zk[i] = rk[i] / diag[i];
  }

  double *xk = x0;
  double *pk = (double *)calloc(N, sizeof(double));
  memcpy(pk, zk, N * sizeof(double));

  double rr = 0.0;
#pragma omp parallel for reduction(+ : rr) default(none) shared(rk, zk, N)
  for (int i = 0; i < N; i++)
  {
    rr += rk[i] * zk[i];
  }

  double *Apk = (double *)malloc(N * sizeof(double));
  int iter = 0;

  while (iter < MAX_ITER)
  {
    memset(Apk, 0, N * sizeof(double));

#pragma omp parallel for default(none) shared(I, J, V, Apk, pk, nnz)
    for (int i = 0; i < nnz; i++)
    {
      int row = I[i];
      int col = J[i];
#pragma omp atomic
      Apk[row] += V[i] * pk[col];
    }

    double denominator = 0.0;
#pragma omp parallel for reduction(+ : denominator) default(none) shared(pk, Apk, N)
    for (int i = 0; i < N; i++)
    {
      denominator += pk[i] * Apk[i];
    }

    double ak = rr / denominator;

#pragma omp parallel for default(none) shared(pk, ak, xk, N)
    for (int i = 0; i < N; i++)
    {
      xk[i] += ak * pk[i];
    }

#pragma omp parallel for default(none) shared(ak, Apk, rk, N)
    for (int i = 0; i < N; i++)
    {
      rk[i] -= ak * Apk[i];
    }

    double rk_norm_sq = 0.0;
#pragma omp parallel for reduction(+ : rk_norm_sq) default(none) shared(rk, N)
    for (int i = 0; i < N; i++)
    {
      rk_norm_sq += rk[i] * rk[i];
    }

    if (sqrt(rk_norm_sq) < TOL)
    {
      printf("Sparse method converged at %d iterations.\n", iter);
      free(pk);
      free(rk);
      free(zk);
      free(Apk);
      free(diag);
      return xk;
    }

#pragma omp parallel for default(none) shared(zk, rk, diag, N)
    for (int i = 0; i < N; i++)
    {
      zk[i] = rk[i] / diag[i];
    }

    double rr_next = 0.0;
#pragma omp parallel for reduction(+ : rr_next) default(none) shared(rk, zk, N)
    for (int i = 0; i < N; i++)
    {
      rr_next += rk[i] * zk[i];
    }

    double bk = rr_next / rr;

#pragma omp parallel for default(none) shared(pk, zk, bk, N)
    for (int i = 0; i < N; i++)
    {
      pk[i] = zk[i] + bk * pk[i];
    }

    rr = rr_next;
    iter++;
  }

  free(rk);
  free(pk);
  free(zk);
  free(Apk);
  free(diag);
  fprintf(stderr, "Sparse method failed to converge after %d iterations\n", iter);
  return xk;
}

double *conjugate_gradient(double *A, double *b, double *x0, int N, double TOL, int MAX_ITER)
{
  // Ax0 = A * x0
  double *Ax0 = (double *)malloc(N * sizeof(double));
#pragma omp parallel for default(none) shared(A, x0, Ax0, N)
  for (int i = 0; i < N; i++)
  {
    double sum = 0.0;
    for (int j = 0; j < N; j++)
      sum += A[i * N + j] * x0[j];
    Ax0[i] = sum;
  }

  // rk = b - Ax0
  double *rk = (double *)malloc(N * sizeof(double));
#pragma omp parallel for default(none) shared(rk, b, Ax0, N)
  for (int i = 0; i < N; i++)
  {
    rk[i] = b[i] - Ax0[i];
  }
  free(Ax0);

  double *zk = (double *)malloc(N * sizeof(double));

  // preconditioner
#pragma omp parallel for default(none) shared(zk, rk, A, N)
  for (int i = 0; i < N; i++)
  {
    // A[i * N + i] is the diagonal element
    zk[i] = rk[i] / A[i * N + i];
  }

  double *xk = x0;
  double *pk = (double *)malloc(N * sizeof(double));

  // pk starts as z0, not r0
  memcpy(pk, zk, N * sizeof(double));

  // rr is now rk^T zk (instead of rk^T rk)
  double rr = 0.0;
#pragma omp parallel for reduction(+ : rr) default(none) shared(rk, zk, N)
  for (int i = 0; i < N; i++)
  {
    rr += rk[i] * zk[i];
  }

  double *Apk = (double *)malloc(N * sizeof(double));
  int iter = 0;

  while (iter < MAX_ITER)
  {
#pragma omp parallel for default(none) shared(A, Apk, pk, N)
    for (int i = 0; i < N; i++)
    {
      double sum = 0.0;
      for (int j = 0; j < N; j++)
        sum += A[i * N + j] * pk[j];
      Apk[i] = sum;
    }

    double denominator = 0.0;
#pragma omp parallel for reduction(+ : denominator) default(none) shared(pk, Apk, N)
    for (int i = 0; i < N; i++)
    {
      denominator += pk[i] * Apk[i];
    }

    // ak = (rk^T zk) / (pk^T A pk)
    double ak = rr / denominator;

    // xk += ak pk
#pragma omp parallel for default(none) shared(pk, ak, xk, N)
    for (int i = 0; i < N; i++)
    {
      xk[i] += ak * pk[i];
    }

    // rk -= ak A pk
#pragma omp parallel for default(none) shared(ak, Apk, rk, N)
    for (int i = 0; i < N; i++)
    {
      rk[i] -= ak * Apk[i];
    }

    // L2 norm
    double rk_norm_sq = 0.0;
#pragma omp parallel for reduction(+ : rk_norm_sq) default(none) shared(rk, N)
    for (int i = 0; i < N; i++)
    {
      rk_norm_sq += rk[i] * rk[i];
    }

    if (sqrt(rk_norm_sq) < TOL)
    {
      printf("Method converged at %d iterations.\n", iter);
      free(pk);
      free(rk);
      free(zk);
      free(Apk);
      return xk;
    }

    // preconditioner
#pragma omp parallel for default(none) shared(zk, rk, A, N)
    for (int i = 0; i < N; i++)
    {
      zk[i] = rk[i] / A[i * N + i];
    }

    // rr_next = r_{k+1}^T z_{k+1}
    double rr_next = 0.0;
#pragma omp parallel for reduction(+ : rr_next) default(none) shared(rk, zk, N)
    for (int i = 0; i < N; i++)
    {
      rr_next += rk[i] * zk[i];
    }

    double bk = rr_next / rr;

    // pk = zk + bk * pk
#pragma omp parallel for default(none) shared(pk, zk, bk, N)
    for (int i = 0; i < N; i++)
    {
      pk[i] = zk[i] + bk * pk[i];
    }

    rr = rr_next;
    iter++;
  }

  free(rk);
  free(pk);
  free(zk);
  free(Apk);
  fprintf(stderr, "Method failed to converge after %d iterations\n", iter);
  return xk;
}