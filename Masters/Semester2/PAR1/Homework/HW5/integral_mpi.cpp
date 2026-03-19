#define _USE_MATH_DEFINES

#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <mpi.h>

double f(double x)
{
  return 4.0 / (1.0 + x * x);
}

double integral_midpoint_allgather(double (*f)(double), int a, int b, long N, int numtasks, int taskid)
{
  if (b == a)
    return 0;
  if (b < a)
    return -integral_midpoint_allgather(f, b, a, N, numtasks, taskid);

  double step = (b - a) / static_cast<double>(N);
  double global_area = 0.0, recv_area;
  double area = 0.0;
  long start = taskid * N / numtasks;
  long end = (taskid + 1) * N / numtasks;
  double curr_mark = a + start * step;

  for (long i = start; i < end; i++)
  {
    double mid = curr_mark + step / 2;
    area += step * f(mid);
    curr_mark += step;
  }
  double *gather_area = (double *)malloc(sizeof(double) * numtasks);
  MPI_Allgather(&area, 1, MPI_DOUBLE, gather_area, 1, MPI_DOUBLE, MPI_COMM_WORLD);

  for (int t = 0; t < numtasks; t++)
  {
    global_area += gather_area[t];
  }

  free(gather_area);

  return global_area;
}

double integral_midpoint_send(double (*f)(double), int a, int b, long N, int numtasks, int taskid)
{
  if (b == a)
    return 0;
  if (b < a)
    return -integral_midpoint_send(f, b, a, N, numtasks, taskid);

  double step = (b - a) / static_cast<double>(N);
  double global_area = 0.0, recv_area;
  double area = 0.0;
  long start = taskid * N / numtasks;
  long end = (taskid + 1) * N / numtasks;
  double curr_mark = a + start * step;

  for (long i = start; i < end; i++)
  {
    double mid = curr_mark + step / 2;
    area += step * f(mid);
    curr_mark += step;
  }

  if (taskid != 0)
  {
    MPI_Send(&area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  else
  {
    global_area = area;
    for (int t = 1; t < numtasks; t++)
    {
      MPI_Recv(&recv_area, 1, MPI_DOUBLE, t, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      global_area += recv_area;
    }
  }

  return global_area;
}

double integral_midpoint_reduce(double (*f)(double), int a, int b, long N, int numtasks, int taskid)
{
  if (b == a)
    return 0;
  if (b < a)
    return -integral_midpoint_reduce(f, b, a, N, numtasks, taskid);

  double step = (b - a) / static_cast<double>(N);
  double global_area = 0.0;
  double area = 0.0;
  long start = taskid * N / numtasks;
  long end = (taskid + 1) * N / numtasks;
  double curr_mark = a + start * step;

  for (int i = start; i < end; i++)
  {
    double mid = curr_mark + step / 2;
    area += step * f(mid);
    curr_mark += step;
  }
  MPI_Reduce(&area, &global_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return global_area;
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int taskid, numtasks;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  if (argc != 2)
  {
    std::cerr << "Invalid argument count. Usage: " << argv[0] << " <N>\n";
    MPI_Finalize();
    return 1;
  }

  long N = std::atol(argv[1]);
  if (N <= 0)
  {
    std::cerr << "Partition size must be positive.\n";
    MPI_Finalize();
    return 1;
  }

  constexpr int a = 0, b = 1;

  auto ti = std::chrono::steady_clock::now();
  double area = integral_midpoint_reduce(f, a, b, N, numtasks, taskid);
  auto tf = std::chrono::steady_clock::now();

  std::chrono::duration<double> t = tf - ti;
  double time = t.count();

  if (taskid == 0)
  {
    std::cout << "===== MPI_REDUCE =====\n";
    std::cout << "Execution time: " << time * 1000 << " ms\n";
    std::cout << "Area: " << std::setprecision(15) << area << std::endl;
    std::cout << "π: " << M_PI << std::endl;
    std::cout << "Error: " << std::setprecision(15) << fabs(area - M_PI) << std::endl;
  }

  ti = std::chrono::steady_clock::now();
  area = integral_midpoint_send(f, a, b, N, numtasks, taskid);
  tf = std::chrono::steady_clock::now();

  t = tf - ti;
  time = t.count();

  if (taskid == 0)
  {
    std::cout << "===== MPI_SEND =====\n";
    std::cout << "Execution time: " << time * 1000 << " ms" << std::endl;
    std::cout << "Area: " << std::setprecision(15) << area << std::endl;
    std::cout << "π: " << M_PI << std::endl;
    std::cout << "Error: " << std::setprecision(15) << fabs(area - M_PI) << std::endl;
  }

  ti = std::chrono::steady_clock::now();
  area = integral_midpoint_allgather(f, a, b, N, numtasks, taskid);
  tf = std::chrono::steady_clock::now();

  t = tf - ti;
  time = t.count();

  if (taskid == 0)
  {
    std::cout << "===== MPI_ALLGATHER =====\n";
    std::cout << "Execution time: " << time * 1000 << " ms" << std::endl;
    std::cout << "Area: " << std::setprecision(15) << area << std::endl;
    std::cout << "π: " << M_PI << std::endl;
    std::cout << "Error: " << std::setprecision(15) << fabs(area - M_PI) << std::endl;
  }
  MPI_Finalize();
}