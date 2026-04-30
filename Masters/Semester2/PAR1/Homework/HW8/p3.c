#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _OPENMP
#include <omp.h>
#else
#include <time.h>
#endif

// access flat array using (i,j) notation
#define IDX(row, col, N) ((row) * (N) + (col))

double get_current_time()
{
#ifdef _OPENMP
  return omp_get_wtime();
#else
  return (double)clock() / CLOCKS_PER_SEC;
#endif
}

double g(double x, double y)
{
  return sin(x) * cos(y);
}

double f(double x, double y)
{
  return -2.0 * sin(x) * cos(y);
}

void solve_poisson_2d_jacobi_l2(int N, double TOL, int MAX_ITER)
{
  int iter;
  double *u = (double *)calloc(N * N, sizeof(double));
  double *u_new = (double *)calloc(N * N, sizeof(double));
  double h = 1.0 / (N - 1); // grid separation

#pragma omp parallel for shared(u, u_new, N, h)
  for (int i = 0; i < N; i++)
  {
    double coord = i * h;

    // bottom edge
    double bottom_val = g(coord, 0.0);
    u[IDX(0, i, N)] = bottom_val;
    u_new[IDX(0, i, N)] = bottom_val;

    // top edge
    double top_val = g(coord, 1.0);
    u[IDX(N - 1, i, N)] = top_val;
    u_new[IDX(N - 1, i, N)] = top_val;

    // left edge
    double left_val = g(0.0, coord);
    u[IDX(i, 0, N)] = left_val;
    u_new[IDX(i, 0, N)] = left_val;

    // right edge
    double right_val = g(1.0, coord);
    u[IDX(i, N - 1, N)] = right_val;
    u_new[IDX(i, N - 1, N)] = right_val;
  }

  double start = get_current_time();
  double l2_sum;
  for (iter = 0; iter < MAX_ITER; iter++)
  {
    l2_sum = 0.0;

#pragma omp parallel for shared(u, u_new, N) reduction(+ : l2_sum)
    for (int i = 1; i < N - 1; i++)
    {
      for (int j = 1; j < N - 1; j++)
      {
        double x = i * h, y = j * h;
        double f_term = h * h * f(x, y);

        u_new[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

        // norm l2
        double diff = fabs(u_new[IDX(i, j, N)] - u[IDX(i, j, N)]);
        l2_sum += diff * diff;
      }
    }

    // swap u, u_new
    double *temp = u;
    u = u_new;
    u_new = temp;

    if (h * sqrt(l2_sum) < TOL)
    {
      printf("Jacobi converged in %d iterations\n", iter);
      printf("L2 Error: %lf\n", h * sqrt(l2_sum));

      break;
    }
  }
  double end = get_current_time();

  printf("Solve took %f seconds\n", end - start);
  if (iter >= MAX_ITER)
  {
    printf("Jacobi failed to converged in %d iterations\n", iter);
    printf("L2 Error: %lf\n", h * sqrt(l2_sum));
  }

  free(u);
  free(u_new);
}

void solve_poisson_2d_jacobi_linf(int N, double TOL, int MAX_ITER)
{
  int iter;
  double *u = (double *)calloc(N * N, sizeof(double));
  double *u_new = (double *)calloc(N * N, sizeof(double));
  double h = 1.0 / (N - 1); // grid separation

#pragma omp parallel for shared(u, u_new, N, h)
  for (int i = 0; i < N; i++)
  {
    double coord = i * h;

    // bottom edge
    double bottom_val = g(coord, 0.0);
    u[IDX(0, i, N)] = bottom_val;
    u_new[IDX(0, i, N)] = bottom_val;

    // top edge
    double top_val = g(coord, 1.0);
    u[IDX(N - 1, i, N)] = top_val;
    u_new[IDX(N - 1, i, N)] = top_val;

    // left edge
    double left_val = g(0.0, coord);
    u[IDX(i, 0, N)] = left_val;
    u_new[IDX(i, 0, N)] = left_val;

    // right edge
    double right_val = g(1.0, coord);
    u[IDX(i, N - 1, N)] = right_val;
    u_new[IDX(i, N - 1, N)] = right_val;
  }

  double max_diff;
  double start = get_current_time();
  for (iter = 0; iter < MAX_ITER; iter++)
  {
    max_diff = 0.0;

#pragma omp parallel for shared(u, u_new, N) reduction(max : max_diff)
    for (int i = 1; i < N - 1; i++)
    {
      for (int j = 1; j < N - 1; j++)
      {
        double x = i * h, y = j * h;
        double f_term = h * h * f(x, y);

        u_new[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

        // norm infinity
        double diff = fabs(u_new[IDX(i, j, N)] - u[IDX(i, j, N)]);
        if (diff > max_diff)
        {
          max_diff = diff;
        }
      }
    }

    // swap u, u_new
    double *temp = u;
    u = u_new;
    u_new = temp;

    if (max_diff < TOL)
    {
      printf("Jacobi converged in %d iterations\n", iter);
      printf("Linf Error: %lf\n", max_diff);

      break;
    }
  }
  double end = get_current_time();
  printf("Solve took %f seconds\n", end - start);
  if (iter >= MAX_ITER)
  {
    printf("Jacobi failed converged in %d iterations\n", iter);
    printf("Linf Error: %lf\n", max_diff);
  }

  free(u);
  free(u_new);
}

void solve_poisson_2d_gs_l2(int N, double TOL, int MAX_ITER)
{
  int iter;
  double *u = (double *)calloc(N * N, sizeof(double));
  double h = 1.0 / (N - 1);

  // initial conditions on boundary
#pragma omp parallel for shared(u, N, h)
  for (int i = 0; i < N; i++)
  {
    double coord = i * h;
    u[IDX(0, i, N)] = g(coord, 0.0);
    u[IDX(N - 1, i, N)] = g(coord, 1.0);
    u[IDX(i, 0, N)] = g(0.0, coord);
    u[IDX(i, N - 1, N)] = g(1.0, coord);
  }

  double l2_sum;
  double start = get_current_time();
  // gauss-seidel loop
  for (iter = 0; iter < MAX_ITER; iter++)
  {
    l2_sum = 0.0;

    // even squares
#pragma omp parallel for shared(u, N) reduction(+ : l2_sum)
    for (int i = 1; i < N - 1; i++)
    {
      for (int j = 1; j < N - 1; j++)
      {
        if ((i + j) % 2 == 0)
        {
          double x = i * h, y = j * h;
          double f_term = h * h * f(x, y);

          double old_val = u[IDX(i, j, N)];
          u[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

          double diff = fabs(u[IDX(i, j, N)] - old_val);
          l2_sum += diff * diff;
        }
      }
    }

    // odd squares
#pragma omp parallel for shared(u, N) reduction(+ : l2_sum)
    for (int i = 1; i < N - 1; i++)
    {
      for (int j = 1; j < N - 1; j++)
      {
        if ((i + j) % 2 != 0)
        {
          double x = i * h, y = j * h;
          double f_term = h * h * f(x, y);

          double old_val = u[IDX(i, j, N)];
          u[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

          double diff = fabs(u[IDX(i, j, N)] - old_val);
          l2_sum += diff * diff;
        }
      }
    }

    if (h * sqrt(l2_sum) < TOL)
    {
      printf("Gauss-Seidel converged in %d iterations\n", iter);
      printf("L2 Error: %lf\n", h * sqrt(l2_sum));
      break;
    }
  }
  double end = get_current_time();
  printf("Solve took %f seconds\n", end - start);
  if (iter >= MAX_ITER)
  {
    printf("Gauss-Seidel failed to converge in %d iterations\n", iter);
    printf("L2 Error: %lf\n", h * sqrt(l2_sum));
  }

  free(u);
}

void solve_poisson_2d_gs_linf(int N, double TOL, int MAX_ITER)
{
  int iter;
  double *u = (double *)calloc(N * N, sizeof(double));
  double h = 1.0 / (N - 1);

  // initial conditions on boundary
#pragma omp parallel for shared(u, N, h)
  for (int i = 0; i < N; i++)
  {
    double coord = i * h;
    u[IDX(0, i, N)] = g(coord, 0.0);
    u[IDX(N - 1, i, N)] = g(coord, 1.0);
    u[IDX(i, 0, N)] = g(0.0, coord);
    u[IDX(i, N - 1, N)] = g(1.0, coord);
  }

  double max_diff;
  double start = get_current_time();
  // gauss-seidel loop
  for (iter = 0; iter < MAX_ITER; iter++)
  {
    max_diff = 0.0;

    // even squares
#pragma omp parallel for shared(u, N) reduction(max : max_diff)
    for (int i = 1; i < N - 1; i++)
    {
      for (int j = 1; j < N - 1; j++)
      {
        if ((i + j) % 2 == 0)
        {
          double x = i * h, y = j * h;
          double f_term = h * h * f(x, y);

          double old_val = u[IDX(i, j, N)];
          u[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

          double diff = fabs(u[IDX(i, j, N)] - old_val);
          if (diff > max_diff)
            max_diff = diff;
        }
      }
    }

    // odd squares
#pragma omp parallel for shared(u, N) reduction(max : max_diff)
    for (int i = 1; i < N - 1; i++)
    {
      for (int j = 1; j < N - 1; j++)
      {
        if ((i + j) % 2 != 0)
        {
          double x = i * h, y = j * h;
          double f_term = h * h * f(x, y);

          double old_val = u[IDX(i, j, N)];
          u[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

          double diff = fabs(u[IDX(i, j, N)] - old_val);
          if (diff > max_diff)
            max_diff = diff;
        }
      }
    }

    if (max_diff < TOL)
    {
      printf("Gauss-Seidel converged in %d iterations\n", iter);
      printf("Linf Error: %lf\n", max_diff);
      break;
    }
  }
  double end = get_current_time();
  printf("Solve took %f seconds\n", end - start);

  if (iter >= MAX_ITER)
  {
    printf("Gauss-Seidel failed to converge in %d iterations\n", iter);
    printf("Linf Error: %lf\n", max_diff);
  }

  free(u);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Invalid argument count. Usage: %s <grid_division> <threads>", argv[0]);
    return -1;
  }
  int N = atoi(argv[1]);
  int t = atoi(argv[2]);
#ifdef _OPENMP
  omp_set_num_threads(t);

  printf("  Number of processors available = %d\n", omp_get_num_procs());
  printf("  Number of threads =              %d\n", omp_get_max_threads());
#endif
  solve_poisson_2d_jacobi_l2(N, 1e-5, 10000);
  solve_poisson_2d_jacobi_linf(N, 1e-5, 10000);
  solve_poisson_2d_gs_l2(N, 1e-5, 10000);
  solve_poisson_2d_gs_linf(N, 1e-5, 10000);
}