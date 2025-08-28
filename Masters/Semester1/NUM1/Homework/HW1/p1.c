#include <stdio.h>
#define lsb 0.0002441406

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
  fprintf(file, "zero,one,two,three,four,five,six,seven\n");

  for (int i = 1; i < 1 << 12; i++) { // loop from i = 1 (we ignore 0), up to 2^12 - 1
    for (int j = 0; j < 1 << 3; j++) { // loop from j = 0 up to 2^3 - 1
      if (j == 7) {
        fprintf(file,"%d\n", i * (1 << j)); 
      }
      else {
        fprintf(file,"%d,", i * (1 << j)); 
      }
    }
  }
}

void print_fractions() {
  FILE *file;
  file = fopen("fractions.csv", "w+");
  fprintf(file, "zero,one,two,three,four,five,six,seven\n");

  for (float i = lsb; i < 1; i += 1) {
    for (int j = 0; j < 1 << 3; j++) {
      
      if (i == 12) {

        fprintf(file,"%d\n", i * (1 << j)); 
      }
      else {
        fprintf(file,"%d,", i * (1 << j)); 
      }
    }
  }

}

int main() {
  // print_representation(); 
  return 0;
}
