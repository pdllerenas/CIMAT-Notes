#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils/conjugate_gradient.h"
#define TOL 1e-5
#define STEPS 200

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Use: %s <nodes> <lambda>\n", argv[0]);
    return 1;
  }

  FILE *fout = fopen("results.csv", "w");
  if (fout == NULL)
  {
    perror("fopen failed");
    return 1;
  }

  int N = atoi(argv[1]);
  double lambda = atof(argv[2]);

  double diag = 1.0 + 2.0 * lambda;
  double off_diag = -lambda;

  double *u = (double *)calloc(N, sizeof(double));
  double *u_prev = (double *)calloc(N, sizeof(double));

  double dx = 1.0 / (N - 1);
  for (int i = 0; i < N; i++)
  {
    double x = i * dx;
    u_prev[i] = 4.0 * x - 4.0 * x * x;
    fprintf(fout, "%f%s", u_prev[i], (i == N - 1) ? "" : ",");
  }
  fprintf(fout, "\n");

  double dt = lambda * dx * dx;
  int steps = (int)ceil(0.2 / dt);

  // initial guess printed
  for (int i = 0; i < N; i++)
  {
    fprintf(fout, "%f%s", u_prev[i], (i == N - 1) ? "" : ",");
  }
  fprintf(fout, "\n");

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  // run cg
  for (int t = 0; t < steps; t++)
  {
    memcpy(u, u_prev, N * sizeof(double));
    u = conjugate_gradient_3diag(diag, off_diag, u, u_prev, N, TOL, N);

    for (int i = 0; i < N; i++)
    {
      fprintf(fout, "%f%s", u[i], (i == N - 1) ? "" : ",");
    }
    fprintf(fout, "\n");

    memcpy(u_prev, u, N * sizeof(double));

    printf("Time step %d completed.\n", t + 1);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_time = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Sequential simulation finished in %.6f seconds.\n", elapsed_time);

  fclose(fout);
  free(u);
  free(u_prev);

  return 0;
}