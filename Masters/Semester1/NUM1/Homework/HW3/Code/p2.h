#include <stdio.h>

double **create_upper_from_file(const char *filename, int *cols, int *rows);
double *solve_upper(double **U, double *b, int dim);
void print_upper_solution(double **U, double *b, double *x, int rows, int cols);
