#include <math.h>
#include <stdio.h>

/*
returns the nth term of the sequence (1+1/n)^n
 */
long double approx_exp(unsigned long int n) { return powl(1.0 + 1.0 / n, n); }

/*
prints the absolute and relative errors to the file inserted
 */
void print_relative_error(FILE *file, double *xn, double x, int dim) {
  fprintf(file, "Absolute Error, Relative Error\n");
  for (int i = 0; i < dim; i++) {
    fprintf(file, "%.15e, %.15e\n", fabs(*xn - x), fabs((*xn - x) / (*xn)));
    xn++;
  }
}
int main() {
  unsigned long long int term = 10;
  short unsigned int pow = 1;       // we use powers of 10, that is, 10^k for 1\leq k \leq 20
  double e_n[20];    // storage of e_n = (1+1/n)^n
  double e = exp(1); // value of e^1 given by the math.h library

  while (pow <= 20) {
    e_n[pow - 1] = approx_exp(term);
    printf("e_{10^{%d}} = %.15lf\n", pow , e_n[pow - 1]); // prints the value of e_n

    term *= 10;
    pow++;
  }
  FILE *file;
  file = fopen("errors.csv",
               "w+"); // the file errors.csv will contain the absolute and
                      // relative errors of the sequence and the value e^1 given
                      // by the math.h library
  print_relative_error(file, e_n, e, pow - 1);
}
