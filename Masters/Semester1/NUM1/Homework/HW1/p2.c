#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SQRT2 1.4142135624

/*

this implementation rewrites the formula into

x_{n+1} = \frac{\sqrt{2}x_{n}}{\sqrt{1+\sqrt{1-4^{1-n}x^{2}_{n}}}}

although using division, this reduces the number of subtractions used,
which leads to less floating point precision errors
accumulating, resulting in the sequence staying at the
convergence point for longer.

 */

double non_naive(int n) {
  FILE *file;
  file = fopen("p2.csv", "w+");
  double x = 2.0; // first term
  double four_pow_one_minus_n = 1.0; // the term 4^{1-n}, divided by 4 each iteration
  for (int i = 3; i < n + 2; i++) {
    x = SQRT2 * x / (sqrt(1 + sqrt(1 - (four_pow_one_minus_n /= 4)* x * x)));
    fprintf(file, "x_%d, %.16lf\n", i, x);
  }
  return x;
}

/*

Naive implementation: this implementation uses the sequence formula

x_{n+1} = 2^{n-1/2} \sqrt{1-\sqrt{1-4^{1-n}x_{n}^{2}}}

This leads to the sequence diverging due to floating point precision
at the term 29, which then spirals into infinity.

 */
double naive(int n) {
  double x = 2.0; // first term
  double two_pow_n_minus_one = SQRT2; // term 2^{n-1/2}, multiplied by 2 each iteration
  double four_pow_one_minus_n = 1.0; // term 4^{1-n}, divided by 4 each iteration
  FILE *file;
  file = fopen("naive_p2.csv", "w+");
  fprintf(file, "x_2, %.16lf\n", x);
  for (int i = 3; i < n + 2; i++) {
    x = (two_pow_n_minus_one *= 2) *
        sqrt(1 - sqrt(1 - (four_pow_one_minus_n /= 4) * x * x)); 
    // Note that the assignation of two_pow_n_minus_one is done before being used by x
    // Similarly with four_pow_one_minus_n 
    fprintf(file, "x_%d, %.16lf\n", i, x);
  }
  return x;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid arguments. Usage: ./p2 (terms)");
    exit(1);
  }
  int terms = atoi(argv[1]);
  non_naive(terms);
  naive(terms);
  return 0;
}
