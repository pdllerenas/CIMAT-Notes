#include <stdio.h>
#include <stdlib.h>
#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "vectors.h"

int main(int argc, char *argv[]) {
  if (argc > 4 || argc < 3) {
    fprintf(stderr,
            "Invalid number of arguments.\nUsage: %s <matrix-file> "
            "<vector-file> [D | U | L]\n",
            argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (!file) {
    fclose(file);
    perror(argv[1]);
    return 1;
  }

  double **matrix = NULL; // space for 2d arrays
  double *diag_matrix =
      NULL;       // diagonal matrices are stored in 1 dimensional arrays
  int cols, rows; // columns and rows for entered matrix
  int vector_dim; // b vector dimension
  FILE *vector_b = fopen(argv[2], "r");
  if (!vector_b) {
    perror(argv[2]);
    return 1;
  }
  printf("Creating vector...\n");
  double *b =
      file_to_vector(vector_b, &vector_dim); // store txt file in pointer
  double *x = NULL;                          // space for solution vector

  if (argc == 4) {
    switch (*argv[3]) {
    case 'D':
      printf("Creating diagonal matrix...\n");

      // if D option is chosen, handle file assuming every entry is 0
      // except the diagonal
      diag_matrix = create_diagonal_from_file(file, &cols, &rows);
      if (cols != vector_dim) {
        fprintf(stderr, "Invalid column size or vector size.\n");
        exit(1);
      }
      printf("Finding solution vector...\n");

      // solve system of equations assuming it is a diagonal matrix
      x = solve_diagonal(diag_matrix, b, rows);

      // print D x = b
      print_diagonal_solution(rows, cols, diag_matrix, x, b);

      // free everything
      free(diag_matrix);
      free(b);
      free(x);
      break;
    case 'L':
      printf("Creating lower traingular matrix...\n");
      matrix = create_lower_from_file(file, &cols, &rows);
      printf("Finding solution vector...\n");
      x = solve_lower(matrix, b, rows);
      print_lower_solution(matrix, x, b, rows, cols);
      for (int i = 0; i < rows; i++) {
        free(matrix[i]);
      }
      free(matrix);
      free(b);
      free(x);
      break;
    case 'U':
      printf("Creating upper traingular matrix...\n");
      matrix = create_upper_from_file(file, &cols, &rows);
      printf("Finding solution vector...\n");
      x = solve_upper(matrix, b, rows);
      print_upper_solution(matrix, b, x, rows, cols);
      for (int i = 0; i < rows; i++) {
        free(matrix[i]);
      }
      free(matrix);
      free(b);
      free(x);
      break;
    default:
      fprintf(stderr,
              "Invalid matrix options. Correct usage: %s <matrix-file> "
              "<vector-file> [D | U | L]",
              argv[0]);
      exit(3);
    }
  }
}
