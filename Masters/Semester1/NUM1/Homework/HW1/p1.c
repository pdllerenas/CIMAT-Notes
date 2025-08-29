#include <stdio.h>
#define lsb 0.0002441406

void print_binary(FILE *file, int n) {
  for (int i = 11; i >= 0; i--) { // 6 bits are enough for numbers up to 63
    int bit = (n >> i) & 1;
    fprintf(file, "%d", bit);
  }
  fprintf(file, ",");
}

/*

16 bit machine:
1 bit for sign
3 bits for exponent
12 bits for mantissa

sgn * 2^exp * mantissa
max. number represented:
0111111111111111 = (2^(12) - 1) * 2^7 = 4095 * 2^7 = 32768

min. number represented:
111111111111111 = -32768

impossible to represent the number 2^12 + 1 = 4097

*/

/*

print_representation() writes to a csv file "binary.csv"
all possible number representations in decimal format.
we then confirm that 4097 is not present.

*/
void print_representation() {
  int curr = 0x0;
  FILE *file;
  file = fopen("binary.csv", "w+");
  /*
    header for the csv, representing
    the exponent of 2 which the mantissa
    is being multiplied by
   */
  fprintf(file, "binary, 2^{0},2^{1},2^{2},2^{3},2^{4},2^{5},2^{6},2^{7}\n");

  for (int i = 1; i < 1 << 12;
       i++) { // loop from i = 1 (we ignore 0), up to 2^12 - 1
    print_binary(file, i);
    for (int j = 0; j < 1 << 3; j++) { // loop from j = 0 up to 2^3 - 1
      if (j == 7) {
        fprintf(file, "%d\n", i * (1 << j));
      } else {
        fprintf(file, "%d,", i * (1 << j));
      }
    }
  }
}

void print_fractions() {
  FILE *file;
  file = fopen("fractions.csv", "w+");
  fprintf(file, "zero,one,two,three,four,five,six,seven\n");
  int curr;
  float i;

  for (i = lsb, curr = 0; i < 1; i += lsb, curr++) {
    print_binary(file, curr);
    for (int j = 0; j < 1 << 3; j++) {

      if (j == 7) {
        fprintf(file, "%f\n", i * (1 << j));
      } else {
        fprintf(file, "%f,", i * (1 << j));
      }
    }
  }
}

int main() {
  print_representation();
  print_fractions();
  return 0;
}
