#include "../../utils/roots/bisection.h"
#include <math.h>
#include <stdio.h>

double f1(double x) { return 3 * exp(-x); }
double f2(double x) { return (2 * x + 3 * exp(-x)) / 3.0; }
double f3(double x) { return 1.05 * x + 3 * exp(-x); }
double f4(double x) { return (x + 3 * exp(-x)) / 2.0; }

int main(int argc, char *argv[]) {

  FILE *fp1 = fopen("p3_1.txt", "w");
  FILE *fp2 = fopen("p3_2.txt", "w");
  FILE *fp3 = fopen("p3_3.txt", "w");
  FILE *fp4 = fopen("p3_4.txt", "w");

  double root_f1 = fixed_point_iteration(1.05, 1e-15, 50, f1, fp1);
  double root_f2 = fixed_point_iteration(1.05, 1e-15, 50, f2, fp2);
  double root_f3 = fixed_point_iteration(1.05, 1e-15, 50, f3, fp3);
  double root_f4 = fixed_point_iteration(1.05, 1e-15, 50, f4, fp4);

  printf("r1 = %lf\n", root_f1);
  printf("r1 = %lf\n", root_f2);
  printf("r1 = %lf\n", root_f3);
  printf("r1 = %lf\n", root_f4);
}
