/*


OWNER: PEDRO DAVID LLERENAS GONZALEZ


 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// iterative methods for root finding (name is a bit misleading, but it contains
// various methods)
#include "../../utils/roots/bisection.h"

// e^x - 4/(2+x)
double f(double x) { return exp(x) - 4 / (2 + x); }

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "Insufficient arguments. Usage: %s <TOLERANCE> <MAX-ITER>.\n", argv[0]);
    exit(1);
  }
  double TOLERANCE = strtod(argv[1], NULL);
  int MAX_ITER = atoi(argv[2]);
  
  double root = bisection_method(0, 1, TOLERANCE, MAX_ITER, f);
  printf("root = %.15lf\n", root);

  // verificacion de raiz
  printf("f(%.15lf) = %.15lf\n", root, f(root));
}
