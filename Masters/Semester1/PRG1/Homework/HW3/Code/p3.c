#include <stdio.h>
#include <stdlib.h>

#include "pgm_io.h"

/*

We basically perform the base change doing floor(input[i][j]/2^{8-bits}) * 2^{8-bits}.

 */
void change_binary_representation(unsigned char **input, int rows, int cols,
                                  unsigned char bits) {
  float factor = 1.0 / (1 << (8-bits));
  int level = 1 << (8 - bits);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      input[i][j] = level * (int)(factor * input[i][j]);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "Invalid number of arguments. Usage: %s <input-file.pgm> "
            "<output-file.pgm> <new-rep>\n",
            argv[0]);
    exit(1);
  }
  if (atoi(argv[3]) >= 8 || atoi(argv[3]) < 1) {
    fprintf(stderr,
            "Invalid binary representation. Must be integer >= 1 and < 8.\n "
            "You entered: %s\n",
            argv[3]);
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
  change_binary_representation(image, rows, cols, atoi(argv[3]));

  // Archivo de salida
  ok = pgmWrite(argv[2], rows, cols, image, NULL);
  free(image);
}
