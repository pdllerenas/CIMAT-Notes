#include <math.h>
#include <stdio.h>

#include "bisection.h"

/*

finds a root of f in the interval (a,b) (up to a tolerance and max iterations)

 */
double bisection_method(double a, double b, double TOLERANCE, int MAX_ITER,
                        double (*f)(double)) {
  if (a > b) {
    // ensure a < b
    return bisection_method(b, a, TOLERANCE, MAX_ITER, f);
  }

  double fa = f(a);
  double fb = f(b);

  if (fa * fb > 0) {
    fprintf(stderr, "Error: f(a) and f(b) must have opposite signs.\n");
    return NAN;
  }

  double c, fc;

  for (int N = 1; N <= MAX_ITER; N++) {
    c = (a + b) / 2.0;
    fc = f(c);

    if ((b - a) / 2.0 < TOLERANCE || fabs(fc) < TOLERANCE) {
      printf("Bisection method converged at iteration %d\n", N);
      return c;
    }

    // narrow the interval of convergence
    if (fa * fc > 0) {
      a = c;
      fa = fc;
    } else {
      b = c;
      fb = fc;
    }
  }

  fprintf(stderr, "Error: Bisection method failed after %d iterations.\n",
          MAX_ITER);
  return NAN;
}
double fixed_point_iteration(double p0, double TOLERANCE, unsigned int MAX_ITER,
                             double (*f)(double), FILE *fp) {

  fprintf(fp, "n xn f(xn) |xn-f(xn)|\n");
  fprintf(fp, "0 %.5e %.5e %.5e\n", p0, f(p0), fabs(p0 - f(p0)));
  for (unsigned int N = 1; N <= MAX_ITER; N++) {
    double p = f(p0);
    fprintf(fp, "%d %.5e %.5e %.5e\n", N, p0, p, fabs(p - p0));
    if (fabs(p - p0) < TOLERANCE) {
      printf("Fixed point iteration method converged at %u iterations.\n", N);
      return p;
    }
    p0 = p;
  }
  fprintf(stderr,
          "Error: Fixed point iteration method failed after %u iterations.\n",
          MAX_ITER);
  return 0;
}

double newton(double p0, double (*f)(double), double (*df)(double), double TOL,
              unsigned int MAX_ITER) {
  for (unsigned int i = 1; i <= MAX_ITER; i++) {
    double d = df(p0);
    if (fabs(d) < 1e-14) {
      fprintf(stderr, "Error: Derivative near zero at iteration %u.\n", i);
      return NAN;
    }

    double p = p0 - f(p0) / d;

    if (fabs(p - p0) < TOL || fabs(f(p)) < TOL) {
      printf("Newton's method converged after %u iterations.\n", i);
      return p;
    }

    p0 = p;
  }

  fprintf(stderr, "Error: Newton's method failed after %u iterations.\n",
          MAX_ITER);
  return NAN;
}
double secant(double p0, double p1, double (*f)(double), double TOL,
              unsigned int MAX_ITER) {
  double q0 = f(p0);
  double q1 = f(p1);
  double p, q;

  for (unsigned int i = 2; i <= MAX_ITER; i++) {
    if (fabs(q1 - q0) < 1e-14) {
      fprintf(stderr, "Error: Secant method failed: division by near-zero.\n");
      return p1;
    }

    p = p1 - q1 * (p1 - p0) / (q1 - q0);
    q = f(p);

    if (fabs(p - p1) < TOL || fabs(q) < TOL) {
      printf("Secant method converged after %d iterations\n", i);

      return p;
    }

    p0 = p1;
    q0 = q1;
    p1 = p;
    q1 = q;
  }

  fprintf(stderr, "Error: Secant method failed after %u iterations.\n",
          MAX_ITER);
  return p1;
}
double false_position(double (*f)(double), double p0, double p1, double TOL,
                      unsigned int MAX_ITER) {
  double q0 = f(p0), q1 = f(p1);

  if (q0 * q1 > 0) {
    fprintf(stderr, "Error: f(p0) and f(p1) must have opposite signs.\n");
    return 0.0;
  }

  double p = p1;
  for (unsigned int k = 1; k <= MAX_ITER; k++) {
    p = p1 - q1 * (p1 - p0) / (q1 - q0);
    double q = f(p);

    if (fabs(p1 - p0) < TOL || fabs(q) < TOL) {
      printf("False position convergeda at %d iterations.\n", k);
      return p;
    }

    if (q * q0 < 0) {
      p1 = p;
      q1 = q;
    } else {
      p0 = p;
      q0 = q;
    }
  }

  fprintf(stderr, "Error: False position method failed after %u iterations.\n",
          MAX_ITER);
  return p;
}
