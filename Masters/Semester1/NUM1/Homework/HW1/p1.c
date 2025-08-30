#include <stdio.h>
#define lsb 0.000244140625

void print_binary(FILE *file, int n) {
  for (int i = 11; i >= 0; i--) {
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
0111111111111111 = (2^(12) - 1) * 2^7 = 4095 * 2^7 = 524160

min. number represented:
111111111111111 = -524160


impossible to represent the number 2^12 + 1 = 4097

*/

/*

print_representation() writes to a csv file "binary.csv"
all possible number representations in decimal format.
we then confirm that 4097 is not present.
*/
void print_representation() {
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
        fprintf(file, "%d\n", i * (1 << j)); // mantissa * 2^j
      } else {
        fprintf(file, "%d,", i * (1 << j));
      }
    }
  }
}

void print_fractions() {
  FILE *file;
  file = fopen("fractions.csv", "w+");
  fprintf(file, "binary, 2^{-2},2^{-2},2^{-1},2^{0},2^{1},2^{2},2^{3}\n");
  int curr;
  double i;
  double mantissa;
  float exp;

  for (i = lsb, curr = 1; i < 1; i += lsb, curr++) {
    print_binary(file, curr);
    for (int j = 0; j < 7; j++) {
      if (j == 0) {
        exp = 1.0/4; // 2^{-2}
        mantissa = i; // implicit bit is 0
      }
      else {
        exp = (j >= 3) ? (1 << (j-3)) : 1.0 / (1 << (3 - j)); // using 2^{j-3} = 1/2^{3-j}
        mantissa = 1 + i; // implicit bit is 1
      }


      if (j == 6) {
        fprintf(file, "%f\n", mantissa * exp);
      } else {
        fprintf(file, "%f,", mantissa * exp);
      }
    }
  }
}

int main() {
  print_representation();
  print_fractions();
  return 0;
}
