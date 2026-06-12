#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <omp.h>
#include <mpi.h>
#include <functional>

#include "search.h"
#include "read.h"
#include "model.h"

using namespace std;

const double PI = 3.14159265358979323846;

struct BenchmarkTask
{
  string name;
  std::function<double(const Point &)> func;
  double custom_lower_bound;
  double custom_upper_bound;
};

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc < 2 || argc > 3)
  {
    if (rank == 0)
      cerr << "Usage: " << argv[0] << " <threads> [parameters_file]\n";
    MPI_Finalize();
    return 1;
  }

  int threads = atoi(argv[1]);
  omp_set_num_threads(threads);

  HParams base_hp;
  if (argc == 3)
  {
    base_hp = read_parameters_data(argv[2]);
  }
  else
  {
    if (rank == 0)
      cerr << "Warning: No parameter file provided. Make sure your base_hp is initialized!\n";
  }

  vector<BenchmarkTask> tasks;
  tasks.push_back({"Rosenbrock", objective_Rosenbrock, -5.0, 10.0});
  tasks.push_back({"Michalewicz", objective_Michalewicz, 0.0, PI});

  if (size < tasks.size() && size != 1)
  {
    if (rank == 0)
    {
      cerr << "Error: You have " << tasks.size() << " benchmarks but only "
           << size << " MPI processes.\n";
    }
    MPI_Finalize();
    return 1;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double global_start_time = MPI_Wtime();

  if (size == 1 || rank < tasks.size())
  {
    int start_idx = (size == 1) ? 0 : rank;
    int end_idx = (size == 1) ? tasks.size() : rank + 1;

    for (int i = start_idx; i < end_idx; i++)
    {
      HParams local_hp = base_hp;
      for (int d = 0; d < local_hp.ndim; d++)
      {
        local_hp.lower_bound[d] = tasks[i].custom_lower_bound;
        local_hp.upper_bound[d] = tasks[i].custom_upper_bound;
      }

      string out_filename = tasks[i].name + "_benchmark.csv";
      ofstream out_file(out_filename);

      double start_time = MPI_Wtime();

      Point best_solution = HarmonicSearch(tasks[i].func, local_hp);

      double end_time = MPI_Wtime();

#ifndef VERBOSE
      cout << "[Rank " << rank << "] " << tasks[i].name
           << " completed in " << (end_time - start_time) << " seconds.\n";

      double final_score = tasks[i].func(best_solution);
      cout << "  -> Best Objective Value Found: " << final_score << "\n";
#endif

      for (size_t j = 0; j < best_solution.size(); ++j)
      {
        out_file << best_solution[j] << (j < best_solution.size() - 1 ? ", " : "");
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double global_end_time = MPI_Wtime();

#ifndef VERBOSE
  if (rank == 0)
  {
    cout << "\n==========================================\n";
    cout << "TOTAL WALL CLOCK TIME: " << (global_end_time - global_start_time) << " seconds.\n";
    cout << "==========================================\n";
  }
#endif

  MPI_Finalize();
  return 0;
}