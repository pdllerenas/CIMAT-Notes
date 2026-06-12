#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <omp.h>
#include <mpi.h>

#include "model.h"
#include "search.h"
#include "read.h"

using namespace std;

struct ObjectiveTask
{
  string name;
  std::function<double(const Point &)> func;
};

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc < 3 || argc > 4)
  {
    cerr << "Invalid argument count. Usage: " << argv[0] << " <threads> <data_file> [parameters_file]\n";
    return 1;
  }

  int threads = atoi(argv[1]);
  omp_set_num_threads(threads);

  string filename = argv[2];
  HParams hp;
  if (argc == 4)
  {
    hp = read_parameters_data(argv[3]);
  }
  vector<DataPoint> experiment_data = read_experimental_data(filename);

  vector<ObjectiveTask> tasks;

  tasks.push_back({"sse", [&experiment_data](const Point &p)
                   { return objective_SSE(p, experiment_data); }});
  tasks.push_back({"sae", [&experiment_data](const Point &p)
                   { return objective_SAE(p, experiment_data); }});
  tasks.push_back({"mae", [&experiment_data](const Point &p)
                   { return objective_MAE(p, experiment_data); }});

  if (size != 1 && size < tasks.size())
  {
    if (rank == 0)
    {
      cerr << "Error: You have " << tasks.size() << " tasks but only "
           << size << " MPI processes were given. Use mpirun -np " << tasks.size() << "\n";
    }
    MPI_Finalize();
    return 1;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double global_start_time = MPI_Wtime();

  if (size == 1)
  {
    for (int i = 0; i < tasks.size(); i++)
    {
      string out_filename = tasks[i].name + "_output.csv";
      ofstream out_file(out_filename);

      double start_time = MPI_Wtime();

      Point x = HarmonicSearch(tasks[i].func, hp);

      double end_time = MPI_Wtime();
      double elapsed_time = end_time - start_time;

      cout << "[Rank 0] Task '" << tasks[i].name
           << "' completed in " << elapsed_time << " seconds.\n";

      for (size_t k = 0; k < x.size(); ++k)
      {
        out_file << x[k] << (k < x.size() - 1 ? ", " : "");
      }
    }
  }
  else
  {
    if (rank < tasks.size())
    {
      string out_filename = tasks[rank].name + "_output.csv";
      ofstream out_file(out_filename);

      MPI_Barrier(MPI_COMM_WORLD);

      double start_time = MPI_Wtime();

      Point x = HarmonicSearch(tasks[rank].func, hp);

      double end_time = MPI_Wtime();
      double elapsed_time = end_time - start_time;
      cout << "[Rank " << rank << "] Task '" << tasks[rank].name
           << "' completed in " << elapsed_time << " seconds.\n";

      for (size_t i = 0; i < x.size(); ++i)
      {
        out_file << x[i] << (i < x.size() - 1 ? ", " : "");
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  double global_end_time = MPI_Wtime();

  if (rank == 0)
  {
    cout << "\n==========================================\n";
    cout << "TOTAL WALL CLOCK TIME: " << (global_end_time - global_start_time) << " seconds.\n";
    cout << "==========================================\n";
  }

  MPI_Finalize();
  return 0;
}