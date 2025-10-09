
#include <stdio.h>
double bisection_method(double a, double b, double TOLERANCE, int MAX_ITER,
                        double (*f)(double));
double fixed_point_iteration(double p0, double TOLERANCE, unsigned int MAX_ITER,
                             double (*f)(double), FILE* fp);
double newton(double p0, double (*f)(double), double (*df)(double), double TOL,
              unsigned int MAX_ITER);
double secant(double p0, double p1, double (*f)(double), double TOL,
              unsigned int MAX_ITER);
double false_position(double (*f)(double), double p0, double p1, double TOL,
                      unsigned int MAX_ITER);
