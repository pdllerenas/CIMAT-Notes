#include "pgm_io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int *image_freq(unsigned char **image, int rows, int cols) {
  unsigned int n_bytes = 0;
  unsigned int n_letters = 0;
  char c, previous_char = 0;
  unsigned int *frequency = calloc(256, sizeof(unsigned int));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      frequency[image[i][j]]++;
    }
  }
  return frequency;
}

double shannon_entropy(unsigned int *frequencies, int space_size) {
  double sum = 0.0;
  for (int i = 0; i < 256; i++) {
    // if probability is zero, the term p log(p) = 0
    if (frequencies[i] == 0) {
      continue;
    }
    double p = frequencies[i] / (double)space_size;
    sum += log2(p) * p;
  }
  return -sum;
}

int main(int argc, char *argv[]) {
  int rows, cols;
  unsigned char **image = pgmRead(argv[1], &rows, &cols);
  unsigned int *freq = image_freq(image, rows, cols);
  double sum = 0.0;
  printf("entropy = %lf\n", shannon_entropy(freq, rows * cols));

  free(freq);
  free(image[0]);
  free(image);
}
