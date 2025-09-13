#include <stdio.h>

double *create_diagonal_from_file(const char* filename, int *cols, int *rows);
double **create_array_from_file(const char* filename, int *cols, int *rows);
double *solve_diagonal(double *D, double *b, int dim);
void print_diagonal_solution(int rows, int cols, double* diag_matrix, double *x, double *b);
