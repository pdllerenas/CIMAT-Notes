#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convolution.h"
#include "kernel_io.h"
#include "pgm_io.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Invalid argument count.\nUsage: %s <input-file-path> <output-file-path> <filter>\n\nAvailable filters:\n'blur3', 'blur5', 'blur7'.\n", argv[0]);
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
  Kernel *(*kfp)();
  if (strcmp(argv[3], "blur3") == 0) {
    kfp = create_mean_3x3;
  } else if (strcmp(argv[3], "blur5") == 0) {
    kfp = create_mean_5x5;
  } else if (strcmp(argv[3], "blur7") == 0) {
    kfp = create_mean_7x7;
  }
  else {
    fprintf(stderr, "Invalid filter: %s\n", argv[3]);
    exit(1);
  }
  print_kernel(kfp());
  
  image_out = apply_filter(image, rows, cols, kfp);

  // Archivo de salida
  ok = pgmWrite(argv[2], rows, cols, image_out, NULL);
  free(image);
  free(image_out);
}
