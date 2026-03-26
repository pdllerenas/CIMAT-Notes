#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>

int main(int argc, char *argv[])
{
  double u0 = 100.0;
  double alpha = 0.1;
  int N = 50;
  long MAX_ITER = 10000;

  if (argc == 5)
  {
    u0 = std::atof(argv[1]);
    alpha = std::atof(argv[2]);
    N = std::atoi(argv[3]);
    MAX_ITER = std::atol(argv[4]);
  }
  else if (argc != 1)
  {
    std::cerr << "Usage: " << argv[0] << " [u0 alpha N MAX_ITER]\n";
    std::cerr << "Running with default values: u0=100, alpha=0.1, N=50, MAX_ITER=20000\n\n";
  }

  std::vector<double> u(N * N, 0.0);
  std::vector<double> u_next(N * N, 0.0);

  int center_i = N / 2;
  int center_j = N / 2;
  int center_idx = center_i + center_j * N;

  // uij - 4 alpha uij
  double self_cooling = 1.0 - 4.0 * alpha;

  auto ti = std::chrono::steady_clock::now();
  for (long iter = 0; iter < MAX_ITER; ++iter)
  {
    for (int j = 1; j < N - 1; ++j)
    {
      for (int i = 1; i < N - 1; ++i)
      {
        // center
        int idx = i + j * N;

        u_next[idx] = (u[idx] * self_cooling) +
                      alpha * (u[idx + 1] + u[idx - 1] + u[idx + N] + u[idx - N]);
      }
    }

    // constant center heat
    u_next[center_idx] = u0;

    std::swap(u, u_next);

    if (iter % 10 == 0)
    {
      std::cout << iter << ',' << u[N + 1] << '\n';
    }
  }
  auto tf = std::chrono::steady_clock::now();
  std::chrono::duration<double> t = tf - ti;
  double time = t.count();
  std::cerr << "Execution time: " << time * 1000 << " ms\n";

  return 0;
}