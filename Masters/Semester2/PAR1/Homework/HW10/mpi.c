#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils/conjugate_gradient.h"
#define TOL 1e-5
#define STEPS 200

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 5)
  {
    if (rank == 0)
      fprintf(stderr, "Please use 5 nodes.\n");
    MPI_Finalize();
    return 1;
  }

  if (argc != 3)
  {
    if (rank == 0)
      fprintf(stderr, "Use: %s <nodes> <lambda>\n", argv[0]);
    MPI_Finalize();
    return 1;
  }

  int N = atoi(argv[1]);
  double lambda = atof(argv[2]);

  double diag = 1.0 + 2.0 * lambda;
  double off_diag = -lambda;

  // rank 0 (master) gets a type 0
  // rank 1-4 (slaves) get a type 1
  int color = (rank == 0) ? 0 : 1;

  // define a communication channel for the workers
  MPI_Comm worker_comm;
  // split the whole world into respective channels
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &worker_comm);

  // split domain (e.g., 62 does not split evenly for 4, so we distribute the remainder)
  int sendcounts[5];
  int displs[5];
  int local_N = 0;

  if (rank == 0)
  {
    int offset = 0;
    sendcounts[0] = 0;
    displs[0] = 0;
    for (int i = 1; i <= 4; i++)
    {
      sendcounts[i] = N / 4 + ((i - 1) < (N % 4) ? 1 : 0);
      displs[i] = offset;
      offset += sendcounts[i];
    }
  }
  else
  {
    local_N = N / 4 + ((rank - 1) < N % 4 ? 1 : 0);
  }

  if (rank == 0)
  {
    FILE *fout = NULL;

    fout = fopen("results.csv", "w");
    if (fout == NULL)
    {
      perror("fopen failed");
      MPI_Abort(MPI_COMM_WORLD, 1);
      return 1;
    }
    double *u_prev = (double *)calloc(N, sizeof(double));
    double dx = 1.0 / (N - 1);
    double dt = lambda * dx * dx;
    int steps = (int)ceil(0.2 / dt);

    for (int i = 0; i < N; i++)
    {
      double x = i * dx;
      u_prev[i] = 4.0 * x - 4.0 * x * x;
      fprintf(fout, "%f%s", u_prev[i], (i == N - 1) ? "" : ",");
    }
    fprintf(fout, "\n");

    printf("Master initialized work\n");
    MPI_Barrier(worker_comm);
    double start_time = MPI_Wtime();
    for (int t = 0; t < steps; t++)
    {
      // send data to slaves
      MPI_Scatterv(u_prev, sendcounts, displs, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      // wait for slaves to finish, then gather their contribution
      MPI_Gatherv(NULL, 0, MPI_DOUBLE, u_prev, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      for (int i = 0; i < N; i++)
      {
        fprintf(fout, "%f%s", u_prev[i], (i == N - 1) ? "" : ",");
      }
      fprintf(fout, "\n");
      printf("Time step: %d\n", t + 1);
    }
    MPI_Barrier(worker_comm);
    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;
    printf("MPI finished in %.6f seconds.\n", elapsed);
  }
  else
  {
    double dx = 1.0 / (N - 1);
    double dt = lambda * dx * dx;
    int steps = (int)ceil(0.2 / dt);
    double *local_u = (double *)calloc(N, sizeof(double));

    for (int t = 0; t < steps; t++)
    {
      MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_u, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      conjugate_gradient_3diag_MPI(diag, off_diag, local_u, local_N, TOL, 1000, worker_comm);
      MPI_Gatherv(local_u, local_N, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    free(local_u);
  }

  MPI_Comm_free(&worker_comm);
  MPI_Finalize();
  return 0;
}