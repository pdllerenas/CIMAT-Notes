#include <math.h>
#include <stdio.h>

void chicharron(double a, double b, double c, double *x1, double *x2) {
  double d = b * b - 4 * a * c;
  if (d < 0) {
    *x1 = *x2 = 0;
    return;
  }
  double s = sqrt(d);
  double two_a = 2 * a;
  *x1 = (-b + s) / (two_a);
  *x2 = (-b - s) / (two_a);
}

int main() {
  double x1, x2;
  chicharron(1.0, 4.6, 1.7, &x1, &x2);
  printf("x_1 = %lf, \t x_2 = %lf\n", x1, x2);
}
