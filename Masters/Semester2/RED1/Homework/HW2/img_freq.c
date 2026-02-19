// Problem 1

#include "pgm_io.h"
#include <stdio.h>
#include <stdlib.h>

int *img_freq(unsigned char **img, int rows, int cols) {
  int *freqs = (int*)calloc(255, sizeof(int));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      freqs[img[i][j]]++;
    }
  }
	return freqs;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Invalid argument count.\n");
  }

  int rows, cols;
  unsigned char **image = pgmRead(argv[0], &rows, &cols);
  if (image == NULL) {
    printf("Error pgmRead\n");
    exit(1);
  }

	int* freqs = img_freq(image, rows, cols);
	for (int i = 0; i < 255; ++i) {
		printf("%d - %d\n", i, freqs[i]);
	}
}
