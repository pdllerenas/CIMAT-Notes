#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../utils/roots/bisection.h"

double f(double x) { return x * (sqrt(x + 1) - sqrt(x)); }

double g(double x) { return x / (sqrt(x + 1) + sqrt(x)); }

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Insufficient arguments. Usage: %s <output-filename1> <output-filename2>\n",
            argv[0]);
    exit(1);
  }
  FILE *fp1 = fopen(argv[1], "w");
  FILE *fp2 = fopen(argv[2], "w");

  for (int n = 1; n <= 20; n++) {
    fprintf(fp1, "%d %lf\n", n, f(pow(10.0, n)));
    fprintf(fp2, "%d %lf\n", n, g(pow(10.0, n)));
  }

  fclose(fp1);
  fclose(fp2);
}
