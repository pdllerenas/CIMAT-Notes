#include <math.h>
#include <stdio.h>

double g1(float x) { return sqrt((2 - x * x * x * x) / 3); }

double g2(float x) { return pow(2 - 3 * x * x, 0.25); }

double g3(float x) { return (2 - x * x * x * x) / (3 * x); }

double g4(float x) {
  double d = (2 - 3 * x * x);
  double t = d / x;
  if (t < 0) {
    return -pow(-t, 1.0 / 3);
  }
  return pow(t, 1.0 / 3);
}

void print_to_csv(double p0, double TOLERANCE, unsigned int MAX_ITER) {
  unsigned int N = 1;
  FILE* file = fopen("p3.csv", "w");
  double p1, p2, p3, p4;
  double p01, p02, p03, p04;
  p01 = p02 = p03 = p04 = p0;
  while (N <= MAX_ITER) {
    p1 = g1(p01); p2 = g2(p02); p3 = g3(p03); p4 = g4(p04);
    fprintf(file, "%lf,", p1);
    fprintf(file, "%lf,", p2);
    fprintf(file, "%lf,", p3);
    fprintf(file, "%lf\n", p4);
    N++;
    p01 = p1;
    p02 = p2;
    p03 = p3;
    p04 = p4;
  }
}

double fixed_point_iteration(double p0, double TOLERANCE, unsigned int MAX_ITER,
                             double (*f)(float)) {
  unsigned int N = 1;
  while (N <= MAX_ITER) {
    double p = f(p0);
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
  print_to_csv(1.0, 0.0001, 20);
  return 0;
}
