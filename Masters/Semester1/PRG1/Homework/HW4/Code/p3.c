#include "pgm_io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*

calculates the frequency of each pixel, assuming there are only 256 possible
values for the given image (which is true for pgm file)

 */
unsigned int *image_freq(unsigned char **image, int rows, int cols) {
  unsigned int *frequency = calloc(256, sizeof(unsigned int));
  if (!frequency) {
    perror("Could not allocate memory.\n");
    exit(1);
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      frequency[image[i][j]]++;
    }
  }
  return frequency;
}

/*

calculates the shannon entropy of an image, assuming 8 bits per pixel.
space_size refers to the total number of pixels in the image

 */
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
  if (image == NULL) {
    printf("Error pgmRead\n");
    exit(1);
  }
  unsigned int *freq = image_freq(image, rows, cols);
  if (!freq) {
    exit(1);
  }
  printf("entropy = %lf\n", shannon_entropy(freq, rows * cols));

  free(freq);
  free(image[0]);
  free(image);
}
