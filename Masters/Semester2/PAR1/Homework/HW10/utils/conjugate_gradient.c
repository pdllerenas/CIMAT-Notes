#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef USE_MPI
#include <mpi.h>
#endif

#include "conjugate_gradient.h"

double *conjugate_gradient_3diag(double diag, double off_diag, double *x0, double *b, int N, double TOL, int MAX_ITER)
{
  // Ax0 = A * x0
  double *Ax0 = (double *)malloc(N * sizeof(double));

  // Dirichlet conditions
  long start = 0;
  long end = N;

  for (int i = start; i < end; i++)
  {
    if (i == 0)
    {
      Ax0[i] = x0[0];
    }
    else if (i == N - 1)
    {
      Ax0[i] = x0[N - 1];
    }
    else
    {
      Ax0[i] = off_diag * x0[i - 1] + diag * x0[i] + off_diag * x0[i + 1];
    }
  }

  // rk = b - Ax0
  double *rk = (double *)malloc(N * sizeof(double));
  double *pk = (double *)malloc(N * sizeof(double));
  double rr = 0.0;

  for (int i = start; i < end; i++)
  {
    rk[i] = b[i] - Ax0[i];
    pk[i] = rk[i];
    rr += rk[i] * rk[i];
  }
  free(Ax0);

  double *xk = x0;
  double *Apk = (double *)malloc(N * sizeof(double));
  int iter = 0;

  while (iter < MAX_ITER)
  {
    if (sqrt(rr) < TOL)
    {
      // printf("Method converged in %d iterations.\n", iter);
      break;
    }

    for (int i = start; i < end; i++)
    {
      if (i == 0)
      {
        Apk[i] = pk[0];
      }
      else if (i == N - 1)
      {
        Apk[i] = pk[N - 1];
      }
      Apk[i] = off_diag * pk[i - 1] + diag * pk[i] + off_diag * pk[i + 1];
    }

    double denominator = 0.0;
    for (int i = 0; i < N; i++)
    {
      denominator += pk[i] * Apk[i];
    }

    // ak = (rk^T rk) / (pk^T A pk)
    double ak = rr / denominator;

    // xk += ak pk
    // rk -= ak A pk
    for (int i = 0; i < N; i++)
    {
      xk[i] += ak * pk[i];
      rk[i] -= ak * Apk[i];
    }

    // rr_next = r_{k+1}^T r_{k+1}
    double rr_next = 0.0;
    for (int i = 0; i < N; i++)
    {
      rr_next += rk[i] * rk[i];
    }

    double bk = rr_next / rr;

    // pk = r_{k+1} + bk * pk
    for (int i = 0; i < N; i++)
    {
      pk[i] = rk[i] + bk * pk[i];
    }

    rr = rr_next;
    iter++;
  }

  if (iter >= MAX_ITER)
  {
    fprintf(stderr, "Method failed to converge in %d iterations\n", iter);
  }

  free(rk);
  free(pk);
  free(Apk);
  return xk;
}

#ifdef USE_MPI
void conjugate_gradient_3diag_MPI(double diag, double off_diag, double *u, int local_N, double TOL, int MAX_ITER, MPI_Comm comm)
{
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  int is_left_boundary = (rank == 0);
  int is_right_boundary = (rank == size - 1);

  // get rank neighbors
  int right_neighbor = (rank < size - 1) ? rank + 1 : MPI_PROC_NULL;
  int left_neighbor = (rank > 0) ? rank - 1 : MPI_PROC_NULL;

  // Ax0 = A * x0
  double *Ax0 = (double *)malloc(local_N * sizeof(double));

  // values of left and right ghost cells for each rank
  double left_ghost = 0.0, right_ghost = 0.0;

  // send and receive data from ghost cells for each neighbor
  MPI_Sendrecv(&u[local_N - 1], 1, MPI_DOUBLE, right_neighbor, 0, &left_ghost, 1, MPI_DOUBLE, left_neighbor, 0, comm, MPI_STATUS_IGNORE);
  MPI_Sendrecv(&u[0], 1, MPI_DOUBLE, left_neighbor, 1, &right_ghost, 1, MPI_DOUBLE, right_neighbor, 1, comm, MPI_STATUS_IGNORE);

  for (int i = 0; i < local_N; i++)
  {
    if (is_left_boundary && i == 0)
    {
      Ax0[i] = u[i];
    }
    else if (is_right_boundary && i == local_N - 1)
    {
      Ax0[i] = u[i];
    }
    else
    {
      double left_value = (i == 0) ? left_ghost : u[i - 1];
      double right_value = (i == local_N - 1) ? right_ghost : u[i + 1];
      Ax0[i] = off_diag * left_value + diag * u[i] + off_diag * right_value;
    }
  }

  // rk = b - Ax0
  double *rk = (double *)malloc(local_N * sizeof(double));
  double *pk = (double *)malloc(local_N * sizeof(double));
  double local_rr = 0.0;

  for (int i = 0; i < local_N; i++)
  {
    rk[i] = u[i] - Ax0[i];
    pk[i] = rk[i];
    local_rr += rk[i] * rk[i];
  }

  double global_rr = 0.0;
  MPI_Allreduce(&local_rr, &global_rr, 1, MPI_DOUBLE, MPI_SUM, comm);
  free(Ax0);

  double *xk = u;
  double *Apk = (double *)malloc(local_N * sizeof(double));
  int iter = 0;

  while (iter < MAX_ITER)
  {
    if (sqrt(global_rr) < TOL)
      break;

    MPI_Sendrecv(&pk[local_N - 1], 1, MPI_DOUBLE, right_neighbor, 0, &left_ghost, 1, MPI_DOUBLE, left_neighbor, 0, comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&pk[0], 1, MPI_DOUBLE, left_neighbor, 1, &right_ghost, 1, MPI_DOUBLE, right_neighbor, 1, comm, MPI_STATUS_IGNORE);

    for (int i = 0; i < local_N; i++)
    {
      if (is_left_boundary && i == 0)
      {
        Apk[i] = pk[i];
      }
      else if (is_right_boundary && i == local_N - 1)
      {
        Apk[i] = pk[i];
      }
      else
      {
        double left_value = (i == 0) ? left_ghost : pk[i - 1];
        double right_value = (i == local_N - 1) ? right_ghost : pk[i + 1];
        Apk[i] = off_diag * left_value + diag * pk[i] + off_diag * right_value;
      }
    }

    double local_denominator = 0.0;
    for (int i = 0; i < local_N; i++)
    {
      local_denominator += pk[i] * Apk[i];
    }

    double global_denominator = 0.0;
    MPI_Allreduce(&local_denominator, &global_denominator, 1, MPI_DOUBLE, MPI_SUM, comm);

    // ak = (rk^T rk) / (pk^T A pk)
    double ak = global_rr / global_denominator;
    double local_rr_next = 0.0;

    // xk += ak pk
    // rk -= ak A pk
    for (int i = 0; i < local_N; i++)
    {
      xk[i] += ak * pk[i];
      rk[i] -= ak * Apk[i];
      local_rr_next += rk[i] * rk[i];
    }

    // rr_next = r_{k+1}^T r_{k+1}
    double global_rr_next = 0.0;
    MPI_Allreduce(&local_rr_next, &global_rr_next, 1, MPI_DOUBLE, MPI_SUM, comm);

    double bk = global_rr_next / global_rr;

    // pk = r_{k+1} + bk * pk
    for (int i = 0; i < local_N; i++)
    {
      pk[i] = rk[i] + bk * pk[i];
    }

    global_rr = global_rr_next;
    iter++;
  }

  free(rk);
  free(pk);
  free(Apk);
}
#endif