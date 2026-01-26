#include <iostream>
#include <omp.h>

int main(void) {
  int x = 5, y = 20;

  omp_set_num_threads(4);

#pragma omp parallel private(x) firstprivate(y)
  {
    x = 10;
    int z = x + y;
    std::cout << z << std::endl;
  }
  return 0;
}
