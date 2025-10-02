#include <stdio.h>
#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/matrix.h"



int main(int argc, char *argv[])
{

  int rows, cols;
  
  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  Vector *b = load_vector_from_txt(argv[2]);
  Vector *x0 = load_vector_from_txt(argv[3]);
  
  double s = matrix_conjugate_with_vector(A, x0);
  printf("%lf\n", s);

  Vector *x = conjugate_gradient(A, b, x0, 1e-2, 100);
}
