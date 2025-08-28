#include <stdio.h>

int main() {
  double epsilon = 1;
  while (1+epsilon > 1) {
    printf("%.22f\n", epsilon);
    epsilon /= 2;
  }
}
