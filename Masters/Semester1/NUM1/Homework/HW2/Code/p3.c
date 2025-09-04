#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double g1(float x) { return sqrt((2 - x * x * x * x) / 3); }

double g2(float x) { return pow(2 - 3 * x * x, 0.25); }

double g3(float x) { return (2 - x * x * x * x) / (3 * x); }

double g4(float x) { return pow((2 - 3 * x * x) / x, 1.0 / 3); }

double fixed_point_iteration(double p0, double TOLERANCE,
                             unsigned int MAX_ITER) {
  unsigned int N = 1;
  while (N <= MAX_ITER) {
    double p = g4(p0);
    if (fabs(p - p0) < TOLERANCE) {
      return p;
    }
    N++;
    p0 = p;
  }
  fprintf(stderr, "Method failed after %d iterations.\n", N);
  exit(1);
}

int main() {
  printf("root = %lf\n", fixed_point_iteration(1, 0.0001, 20));
  return 0;
}
