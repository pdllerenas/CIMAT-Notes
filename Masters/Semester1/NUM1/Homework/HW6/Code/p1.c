#include <stdio.h>

#include "../../utils/linalg/eigenvalues.h"
#include "../../utils/linalg/matrix_io.h"
#include "../../utils/linalg/matrix.h"



int main(int argc, char *argv[])
{

  int rows, cols;
  
  Matrix *A = load_matrix_from_txt(argv[1], &rows, &cols);
  Matrix *phi_0 = load_matrix_from_txt(argv[2], &rows, &cols);


  subspace_iteration(A, phi_0, 1e-6, 2000);
}
