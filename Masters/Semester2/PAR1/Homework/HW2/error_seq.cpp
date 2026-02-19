#include <vector>
#include <iostream>
#include <chrono>

std::vector<float> RandomMatrix(int N, int M)
{
  std::vector<float> matrix(N * M);
  for (int i = 0; i < N * M; ++i)
  {
    matrix[i] = static_cast<float>(rand()) / RAND_MAX; // Random float between 0 and 1
  }
  return matrix;
}

int main(void)
{
  int N, M;
  std::cout << "Enter the dimensions of the matrix (N M): ";
  std::cin >> N >> M;

  int seed;
  std::cout << "Enter a seed: ";
  std::cin >> seed;

  srand(seed);

  constexpr int REPEAT = 100;

  auto benchmark = [&](const std::vector<float> &A,
                       const std::vector<float> &B,
                       const std::string &label)
  {
    float E = 0.0f;

    // Warm-up (not measured)
    for (int i = 0; i < N * M; ++i)
      E += (A[i] - B[i]) * (A[i] - B[i]);

    E = 0.0f;

    auto start = std::chrono::high_resolution_clock::now();

    for (int repeat = 0; repeat < REPEAT; ++repeat)
    {
      for (int i = 0; i < N * M; ++i)
        E += (A[i] - B[i]) * (A[i] - B[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();

    double total_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << label << "\n";
    std::cout << "  Avg time: " << total_ms / REPEAT << " milliseconds\n";
    std::cout << "  E: " << E << "\n\n";
  };

  auto I1 = RandomMatrix(N, M);
  auto I2 = RandomMatrix(N, M);

  benchmark(I1, I2, "Random matrix");
 
}