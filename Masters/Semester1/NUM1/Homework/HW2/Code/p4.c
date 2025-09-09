#include <math.h>
#include <stdio.h>

#define TOLERANCE 0.0001
#define N_MAX_ITER 100

double newton(double p0, double (*f)(double), double (*df)(double), double TOL,
              unsigned int MAX_ITER) {
  unsigned int i = 1;
  while (i < MAX_ITER) {
    double p = p0 - f(p0) / df(p0);
    if (fabs(p - p0) < TOL) {
      return p;
    }
    i++;
    p0 = p;
  }
  fprintf(stderr, "Newton's method failed after N = %d iterations.\n",
          MAX_ITER);
  return 0;
}

double secant(double p0, double p1, double (*f)(double), double TOL,
              unsigned int MAX_ITER) {
  int i = 2;
  double q0 = f(p0), q1 = f(p1);
  double p = p1 - q1 * (p1 - p0) / (q1 - q0);
  while (i < MAX_ITER) {
    if (fabs(p - p0) < TOL) {
      return p;
    }
    i++;
    p0 = p1;
    q0 = q1;
    p1 = p;
    q1 = f(p);
  }
  fprintf(stderr, "Secant's method failed after N = %d iterations.\n",
          MAX_ITER);
  return 0;
}

double false_position(double p0, double p1, double (*f)(double), double TOL,
                      unsigned int MAX_ITER) {
  int i = 2;

  double q0 = f(p0), q1 = f(p1);
  double p = p1 - q1 * (p1 - p0) / (q1 - q0);
  while (i < MAX_ITER) {
    if (fabs(p - p0) < TOL) {
      return p;
    }
    i++;
    double q = f(p);
    if (q * q0 < 0) {
      p0 = p1;
      q0 = q1;
    }
    p1 = p;
    q1 = q;
  }

  fprintf(stderr, "False position's method failed after N = %d iterations.\n",
          MAX_ITER);
  return 0;
}

double dP(double lambda) {
  double e_lambda = exp(lambda);
  return 1e6 * e_lambda +
         (435e3 / lambda * lambda) * (e_lambda * (lambda - 1) + 1);
}

double P(double lambda) {
  double e_lambda = exp(lambda);
  return -1564000 + 1e6 * e_lambda + (435e3 / lambda) * (e_lambda - 1);
}

int main() {
  double r = newton(0.01, P, dP, TOLERANCE, N_MAX_ITER);
  printf("newton found root at %lf\n", r);
  printf("P(%lf) = %lf\n", r, P(r));
  double r_sec = secant(0.01, 0.2, P, TOLERANCE, N_MAX_ITER);
  printf("secant found root at %lf\n", r_sec);
  printf("P(%lf) = %lf\n", r_sec, P(r_sec));
  double r_fp = false_position(0.01, 0.2, P, TOLERANCE, N_MAX_ITER);
  printf("false position found root at %lf\n", r_fp);
  printf("P(%lf) = %lf\n", r_fp, P(r_fp));
  return 0;
}
