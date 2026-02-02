#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <vector>

int main(void) {
  int N;
  std::cout << "size of vector: " << std::endl;
  std::cin >> N;
  if (N <= 0) {
    std::cerr << "Invalid size" << std::endl;
  }

  std::vector<int> v;
  v.reserve(N);

  std::cout << "elements of vector: " << std::endl;
  int i = N;
  while (i--) {
    int a;
    std::cin >> a;
    v.push_back(a);
  }
  omp_set_num_threads(omp_get_max_threads());
  int sum = 0;
  // reduction solves the race condition for us when editing sum's value
#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < N; i++) {
    sum += v[i];
  }

  std::cout << "sum: " << sum << std::endl;

  return 0;
}
