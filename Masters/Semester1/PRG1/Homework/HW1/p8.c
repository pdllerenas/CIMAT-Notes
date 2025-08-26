#include <stdio.h>
#include <stdlib.h>

// NOTE: usage of long ints was merely to check
// difference between algorithms. No substantial
// time difference was found with the current
// implementation.


// gcd obtained using euclid's algorithm
// a = q_1 b + r_1
// b = q_2 r_1 + r_2
// r_1 = q_3 r_2 + r_3
// ...
// r_n = q_{n-2} r_{n-1}
// we iterate until remainder is zero
long int gcd(long int a, long int b) {
  // we assume a > b
  if (a < b) {
    return gcd(b, a);
  }
  long int temp;
  while (b) {
    temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

// returns minimum between two numbers
long int min(long int a, long int b) { return (a < b) ? a : b; }

// slight modification to euclid's algorithm, where
// the residue may be negative, and one takes the remainder
// closes to zero. The number of steps is reduced, but more
// comparisons must be done
long int least_remainder_gcd(long int a, long int b) {
  if (a < b) {
    return least_remainder_gcd(b, a);
  }
  long int temp, r;
  while (b && b != a) {
    temp = b;
    r = a % b;
    b = min(labs(r - b), r); // calculates the remainder closest to 0. 
    a = temp;
  }
  return a;
}

int main() {
  long int a, b, g;
  if (scanf("%ld/%ld", &a, &b) == 2) {
    g = least_remainder_gcd(a, b);
    printf("%ld/%ld\n", a/g, b/g);
  }
}
