#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convolution.h"
#include "kernel_io.h"
#include "pgm_io.h"

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr,
            "Invalid argument count.\nUsage: %s <input-file-path> "
            "<output-file-path> <filter> <window-size>\n\nAvailable "
            "filters:\n'mean', 'median', 'gaussian',\n ",
            argv[0]);
    exit(1);
  }
  int k_dim = atoi(argv[4]); // windows size for kernel
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
  Kernel *(*gaussian_kernel)() = NULL; // fn pointer for kernel creation
  Kernel *(*mean_kernel)(int) = NULL; // fn pointer for nxn mean kernel creation

  // set fn pointer to appropriate one, considering k_dim
  if (strcmp(argv[3], "gaussian") == 0) {
    if (k_dim == 3) {
      gaussian_kernel = create_gaussian_3x3;
    } else if (k_dim == 5) {
      gaussian_kernel = create_gaussian_5x5;
    } else if (k_dim == 7) {
      gaussian_kernel = create_gaussian_7x7;
    } else {
      fprintf(stderr, "Invalid gaussian kernel dimension. Try: 3, 5, or 7.\n");
      exit(1);
    }
  } else if (strcmp(argv[3], "mean") == 0) {
    mean_kernel = create_mean_nxn;
  } else if (strcmp(argv[3], "median") == 0) {
    // median filter has no kernel, so we directly edit the image file
    image_out = median_filter_nxn(image, rows, cols, k_dim);
  } else {
    fprintf(stderr, "Invalid filter: %s\n", argv[3]);
    exit(1);
  }
  if (gaussian_kernel != NULL) {
    image_out = apply_gaussian(image, rows, cols, gaussian_kernel);
  } else if (mean_kernel != NULL) {
    image_out = apply_mean(image, rows, cols, mean_kernel, k_dim);
  }

  // Archivo de salida
  ok = pgmWrite(argv[2], rows, cols, image_out, NULL);
  free(image[0]);
  free(image);
  free(image_out[0]);
  free(image_out);
}
