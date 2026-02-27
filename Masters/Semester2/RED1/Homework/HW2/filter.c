#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "convolution.h"
#include "kernel_io.h"
#include "pgm_io.h"

int main(int argc, char *argv[])
{
  if (argc != 6)
  {
    fprintf(stderr,
            "Invalid argument count.\nUsage: %s <input-file-path> "
            "<output-file-path> <filter> <window-size> <processes>\n\nAvailable "
            "filters:\n'mean', 'median'\n ",
            argv[0]);
    exit(1);
  }
  int k_dim = atoi(argv[4]); // windows size for kernel
  if (k_dim != 3 && k_dim != 5 && k_dim != 9)
  {
    fprintf(stderr, "Invalid window size.\n");
    exit(1);
  }
  int p = atoi(argv[5]);
  if (p <= 0)
  {
    fprintf(stderr, "Invalid process quantity.\n");
    exit(1);
  }
  int i, j, max, ok, rows, cols;
  int count = 0;
  unsigned char **image;

  // Archivo de entrada
  image = pgmRead(argv[1], &rows, &cols);
  if (image == NULL)
  {
    fprintf(stderr, "Error pgmRead\n");
    exit(1);
  }

  // Procesamiento de la imagen: ** INCLUIR SU CODIGO AQUI **
  unsigned char **image_out;
  Kernel *(*mean_kernel)(int) = NULL; // fn pointer for nxn mean kernel creation

  // set fn pointer to appropriate one, considering k_dim
  if (strcmp(argv[3], "mean") == 0)
  {
    mean_kernel = create_mean_nxn;
  }
  else if (strcmp(argv[3], "median") == 0)
  {
    // median filter has no kernel, so we directly edit the image file
    clock_t start_t, end_t;
    double total_t;

    start_t = clock();
    image_out = median_filter_nxn(image, rows, cols, k_dim, p);
    end_t = clock();

    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %f seconds\n", total_t);
  }
  else
  {
    fprintf(stderr, "Invalid filter: %s\n", argv[3]);
    exit(1);
  }
  if (mean_kernel != NULL)
  {
    clock_t start_t, end_t;
    double total_t;

    start_t = clock();
    image_out = apply_mean(image, rows, cols, mean_kernel, k_dim, p);
    end_t = clock();

    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %f seconds\n", total_t);
  }

  // Archivo de salida
  ok = pgmWrite(argv[2], rows, cols, image_out, NULL);
  if (!ok)
  {
    fprintf(stderr, "Error writing output image\n");
  }
  free(image[0]);
  free(image);
  free(image_out[0]);
  free(image_out);
}