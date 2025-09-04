#include <stdlib.h>
#include <stdio.h>

double f(double x) {
  return x*x*x+x-6;
}

double bisection_method(double a, double b, double TOLERANCE, int MAX_ITER) {
  if (a > b) {
    return bisection_method(b, a, TOLERANCE, MAX_ITER);
  }
  // if ((fa < 0 && fb < 0) || (fa > 0 && fb > 0))  {
  //   fprintf(stderr, "Failed to apply method.\n");
  //   exit(1);
  // }
  int N = 1;
  while (N <= MAX_ITER) {
    double c = (a+b)/2;
    if (f(c) == 0 || (b-a)/2<TOLERANCE) {
      printf("Root found at iter = %d\n", N);

      return c;
    }
    N++;
    if ((f(c) < 0 && f(a) < 0) || (f(c) > 0 && f(a) > 0)) {
      a = c;
    } else{
      b = c;
    }
  }
  fprintf(stderr, "Method failed. MAX_ITER exceeded.\n");
  exit(1);
}

int main() {
  printf("root at x = %lf\n", bisection_method(1.55, 1.75, 0.0001, 100));

}
