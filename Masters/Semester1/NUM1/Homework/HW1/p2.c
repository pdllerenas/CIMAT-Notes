#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SQRT2 1.4142135624

double xn(int n) {
  double x = 2.0;
  double two_pow_n_minus_one = SQRT2;
  double two_pow_two_minus_two_n = 1.0;
  printf("x_2 = %lf\n", x);
  for (int i = 3; i < n + 2; i++) {
    x = (two_pow_n_minus_one *= 2) *
        sqrt(1 - sqrt(1 - (two_pow_two_minus_two_n /= 4) * x * x));
    printf("x_%d = %lf\n", i, x);
  }
  return x;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid arguments. Usage: ./p2 (terms)");
    exit(1);
  }
  int terms = atoi(argv[1]);
  xn(terms);
  return 0;
}
