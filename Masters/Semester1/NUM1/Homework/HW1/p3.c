#include <stdio.h>

int main() {
  double epsilon = 1;
  int iter = -1;
  while (1+epsilon > 1) {
    iter++;
    printf("%.22f\n", epsilon);
    epsilon /= 2;
  }
  printf("%d\n", iter);
}
