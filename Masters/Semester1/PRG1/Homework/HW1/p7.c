#include <stdio.h>
#include <stdlib.h>

#define LOOP_LIMIT 10

int main(int argc, char *argv[]) {
  float decimal;
  unsigned int base;

  printf("Enter deciaml and base to convert to.\n");
  printf("For example: 24.3 5 converts 24.3 to base 5.\n");
  printf("To quit, press Ctrl+c\n");
  while (1) {
    if(scanf("%f %ui", &decimal, &base) != 2) {
      fprintf(stderr, "Invalid decimal or base.\n");
      exit(1);
    }

    // separate into integer and fractional parts of decimal
    int d_int = (int)decimal;
    float d_frac = decimal - d_int;

    // we first process the integral part of the input
    int q = d_int, i = 0, j;
    div_t d;

    // store items in list for printing. a very rough estimate of the amount
    // of space needed for the integer part is d_int / base. 
    char result[q / base];
    do {
      d = div(q, base);
      q = d.quot;
      result[i] = d.rem + '0'; // store remainder for later printing

      i++;
    } while (q > 0); // the cycle ends when we obtain a quotient of 0
    printf("%f base 10 = ", decimal);
    // print items in list in reverse order (starting from last recorded and not last index)
    j = i - 1;
    while (j >= 0) {
      printf("%c", result[j]);
      j--;
    }

    // process fractional part of the decimal
    int iter = 0, integral_part;
    float frac = d_frac;

    if (d_frac > 0) {
      printf(".");
      // multiply fractional part until 0
      // or iterations exceed our set limit
      // the integral parts that arise are 
      // the ones we take for the representation
      while (iter < LOOP_LIMIT && frac) {
        frac *= base;
        integral_part = (int)frac;
        frac = frac - integral_part;

        iter++;
        printf("%d", integral_part);
      }
    }

    printf(" base %i", base);
    printf("\n");
  }
}
