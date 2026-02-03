#include <iostream>
#include <omp.h>
#include <vector>

int main(void) {
  int N;
  std::cout << "size of matrix: " << std::endl;
  std::cin >> N;
  if (N <= 0) {
    std::cerr << "Invalid size" << std::endl;
  }

  std::vector<int> m1;
  std::vector<int> m2;
  m1.reserve(N * N);
  m2.reserve(N * N);

  std::cout << "elements of matrix 1: " << std::endl;
  int i = N * N;
  while (i--) {
    int a;
    std::cin >> a;
    m1.push_back(a);
  }

  std::cout << "elements of matrix 2: " << std::endl;
  i = N * N;
  while (i--) {
    int a;
    std::cin >> a;
    m2.push_back(a);
  }
  omp_set_num_threads(omp_get_max_threads());

  std::vector<int> res(N * N);


// Paralelize the outer loop. Parallelizing the inner loops is disabled on compilation:
// http://ppc.cs.aalto.fi/ch3/nested/
// Other alternatives are using collapse or enabling nested parallelization
#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int sum = 0;
      for (int k = 0; k < N; k++) {
        sum += m1[N * i + k] * m2[N * k + j];
      }
      res[N * i + j] = sum;
    }
  }

	std::cout << "result:\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << res[N * i + j] << " ";
    }
    std::cout << '\n';
  }

  return 0;
}
