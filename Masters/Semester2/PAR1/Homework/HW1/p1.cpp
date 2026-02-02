#include <omp.h>
#include <stdio.h>

int main(void) {
  int N = 10;
  int x = 1;
  int y[N]; // 10 elements
  omp_set_num_threads(5);
#pragma omp parallel for firstprivate(x)
  for (int i = 0; i < N; i++) {
    y[i] = x + i;
    x = i; // since this is private from other threads, it should only be
           // affected by the current thread information
  }

  printf("x = %d\n", x);
  for (int i = 0; i < N; i++) {
    printf("y[%d] = %d\n", i, y[i]);
  }
  return 0;
}
//       t0    t1    t2    t3     t4
// y = [1, 1, 3, 5, 5, 9, 7, 13, 9, 17]
// x = 1 because x is firstprivate. If it were lastprivate, it would be 9, but
// assignment to y[0] would be undefined behavior
