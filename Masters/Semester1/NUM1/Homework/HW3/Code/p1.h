#include <stdio.h>

double *create_diagonal_from_file(FILE *file, int *cols, int *rows);
double **create_array_from_file(FILE *file, int *cols, int *rows);
double *solve_diagonal(double *D, double *b, int dim);
void print_diagonal_solution(double rows, double cols, double* diag_matrix, double *x, double *b);
