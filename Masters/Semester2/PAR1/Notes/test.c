#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void openmp_hello(void);

int main(int argc, char *argv[]) {
  int thread_count = strtol(argv[1], NULL, 10);
#pragma omp parallel num_threads(thread_count)
  openmp_hello();
  return 0;
}

void openmp_hello(void) {
  int my_rank = omp_get_thread_num();
  int thread_count = omp_get_num_threads();

  printf("Hola desde el thread %d de %d\n", my_rank, thread_count);
}
