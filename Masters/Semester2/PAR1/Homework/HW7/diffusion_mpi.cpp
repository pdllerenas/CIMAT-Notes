#include <chrono>
#include <iostream>
#include <mpi.h>
#include <vector>

#include "SaveParaview.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int N = 50;
  double u0 = 100.0;
  double alpha = 0.1;
  long MAX_ITER = 10000;

  if (argc == 5) {
    u0 = std::atof(argv[1]);
    alpha = std::atof(argv[2]);
    N = std::atoi(argv[3]);
    MAX_ITER = std::atol(argv[4]);
  }

	int print_freq = MAX_ITER / 20;

  int col_size = N / size;        // real columns size
  int local_width = col_size + 2; // add 2 for ghost columns

  // left neighbor is null if no process behind it (left edge)
  int left_neighbor = (rank == 0) ? MPI_PROC_NULL : rank - 1;

  // right neighbor is null if no process after it (right edge)
  int right_neighbor = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

  MPI_Datatype columntype;
  MPI_Type_vector(N, 1, local_width, MPI_DOUBLE, &columntype);
  MPI_Type_commit(&columntype);

  std::vector<double> u(N * local_width, 0.0);
  std::vector<double> u_next(N * local_width, 0.0);

  // determine which process owns center so we can set it to initial value
  int global_center = N / 2;
  int global_start_col = rank * col_size;
  bool owns_center = (global_start_col <= global_center) &&
                     (global_center < global_start_col + local_width);
  int local_center_idx = -1;

  if (owns_center) {
    int local_center_col =
        global_center - global_start_col + 1; // +1 for left ghost col
    local_center_idx = local_center_col + global_center * local_width;
  }

  double self_cooling = 1.0 - 4.0 * alpha;

  auto ti = std::chrono::steady_clock::now();

  for (long iter = 0; iter < MAX_ITER; iter++) {
    for (int j = 1; j < N - 1; j++) {
      for (int i = 1; i <= col_size; i++) {
        int global_col = global_start_col + (i - 1);

        // ignore edges
        if (global_col == 0 || global_col == N - 1)
          continue;

        int idx = i + j * local_width;

        double left = u[idx - 1];
        double right = u[idx + 1];
        double down = u[idx - local_width];
        double up = u[idx + local_width];

        u_next[idx] =
            (u[idx] * self_cooling) + alpha * (left + right + up + down);
      }
    }

    // set center cell to contant at all iterations
    if (owns_center) {
      u_next[local_center_idx] = u0;
    }

    std::swap(u, u_next);
    // exchange info left column with right column from previous process
    MPI_Sendrecv(&u[1], 1, columntype, left_neighbor, 0, &u[col_size + 1], 1,
                 columntype, right_neighbor, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

    // exchange info right column with left column from next process
    MPI_Sendrecv(&u[col_size], 1, columntype, right_neighbor, 1, &u[0], 1,
                 columntype, left_neighbor, 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

    if (iter % print_freq == 0 || iter == MAX_ITER - 1) {
      if (rank == 0) {
        std::vector<double> x(N), y(N);
        for (int i = 0; i < N; i++) {
          x[i] = (double)i / (N - 1);
          y[i] = (double)i / (N - 1);
        }
        std::vector<std::vector<double>> global_U(N,
                                                  std::vector<double>(N, 0.0));

        for (int j = 0; j < N; j++) {
          for (int i = 1; i <= col_size; i++) {
            int global_col = i - 1;
            global_U[global_col][j] = u[i + j * local_width];
          }
        }

        std::vector<double> recv_buffer(N * local_width);
        for (int p = 1; p < size; p++) {
          MPI_Recv(recv_buffer.data(), N * local_width, MPI_DOUBLE, p, 0,
                   MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          int p_start_col = p * col_size;
          for (int j = 0; j < N; j++) {
            for (int i = 0; i < col_size; i++) {
              int global_col = p_start_col + (i - 1);
              global_U[global_col][j] = recv_buffer[i + j * local_width];
            }
          }
        }
        save_paraview(N, N, x, y, (double)iter, global_U, iter / print_freq, "output");
      } else {
        MPI_Send(u.data(), N * local_width, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      }
    }
  }

  auto tf = std::chrono::steady_clock::now();
  std::chrono::duration<double> t = tf - ti;
  double time = t.count();

  if (rank == 0) {
    std::cerr << "Execution time: " << time * 1000 << " ms\n";
  }

  MPI_Type_free(&columntype);
  MPI_Finalize();
  return 0;
}
