#include <math.h>
#include <stdio.h>

double g1(float x) { return sqrt((2 - x * x * x * x) / 3); }

double g2(float x) { return pow(2 - 3 * x * x, 0.25); }

double g3(float x) { return (2 - x * x * x * x) / (3 * x); }

double g4(float x) { 
  double d = (2 - 3*x*x);
  double t = d / x;
  if (t < 0) {
    return -pow(-t, 1.0/3);
  }
  return pow(t, 1.0/3);
}

double fixed_point_iteration(double p0, double TOLERANCE, unsigned int MAX_ITER,
                             double (*f)(float)) {
  unsigned int N = 1;
  while (N <= MAX_ITER) {
    double p = f(p0);
    printf("p_%d = %lf\n", N, p);
    if (fabs(p - p0) < TOLERANCE) {
      return p;
    }
    N++;
    p0 = p;
  }
  fprintf(stderr, "Method failed after %d iterations.\n", N);
  return 0;
}

int main() {
  printf("g1: root = %lf\n", fixed_point_iteration(1.0, 0.0001, 20, g1));
  printf("g2: root = %lf\n", fixed_point_iteration(1.0, 0.0001, 20, g2));
  printf("g3: root = %lf\n", fixed_point_iteration(1.0, 0.0001, 20, g3));
  printf("g4: root = %lf\n", fixed_point_iteration(1.0, 0.0001, 20, g4));
  return 0;
}
