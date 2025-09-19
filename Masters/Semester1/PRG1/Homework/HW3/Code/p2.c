#include "convolution.h"
#include "kernel_io.h"
#include "pgm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "Invalid argument count.\nUsage: %s <input-file-path> "
            "<output-file-path> <filter>\n\nAvailable filters:\n'scharr', "
            "'gradient', 'laplacian4', 'laplacian20'.\n",
            argv[0]);
    exit(1);
  }
  int i, j, max, ok, rows, cols;
  int count = 0;
  unsigned char **image;

  // Archivo de entrada
  image = pgmRead(argv[1], &rows, &cols);
  if (image == NULL) {
    printf("Error pgmRead\n");
    exit(1);
  }

  // Procesamiento de la imagen: ** INCLUIR SU CODIGO AQUI **
  unsigned char **image_out;
  Kernel *(*kfp1)() = NULL;
  Kernel *(*kfp2)() = NULL;

  if (strcmp(argv[3], "scharr") == 0) {
    kfp1 = create_vertical_schar;
    kfp2 = create_horizontal_schar;
    if (kfp1 != NULL && kfp2 != NULL) {
      image_out = apply_sobel(image, rows, cols, kfp1, kfp2);
    }
  } else if (strcmp(argv[3], "gradient") == 0) {
    image_out = apply_gradient(image, rows, cols);
  } else if (strcmp(argv[3], "laplacian4") == 0) {
    kfp1 = create_laplacian_4;
    if (kfp1 != NULL) {
      image_out = apply_gaussian(image, rows, cols, kfp1);
    }
  } else if (strcmp(argv[3], "laplacian20") == 0) {
    kfp1 = create_laplacian_20;
    if (kfp1 != NULL) {
      image_out = apply_gaussian(image, rows, cols, kfp1);
    }
  } else {
    fprintf(stderr, "Invalid filter: %s\n", argv[3]);
    exit(1);
  }

  // Archivo de salida
  ok = pgmWrite(argv[2], rows, cols, image_out, NULL);
  free(image[0]);
  free(image);

  free(image_out[0]);
  free(image_out);
}
