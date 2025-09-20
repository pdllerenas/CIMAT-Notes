#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_io.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr,
            "Invalid argument count. \nUsage: %s <file-input> <file-size>",
            argv[0]);
    exit(1);
  }
  int rows, cols;
  rows = cols = atoi(argv[2]);

  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  A->element_size = sizeof(double);

  Matrix *U = matrix_create_double(rows, cols),
         *L = matrix_create_double(rows, cols);

  crout(A, L, U, A->rows);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->cols; j++) {
      printf("%lf ", ((double *)A->data)[i * rows + j]);
    }
    printf("\t");

    for (int j = 0; j < A->cols; j++) {
      printf("%lf ", ((double *)L->data)[i * rows + j]);
    }
    printf("\t");
    for (int j = 0; j < A->cols; j++) {
      printf("%lf ", ((double *)U->data)[i * rows + j]);
    }
    printf("\n");
  }

  free(A->data);
  free(A);
  return EXIT_SUCCESS;
}
