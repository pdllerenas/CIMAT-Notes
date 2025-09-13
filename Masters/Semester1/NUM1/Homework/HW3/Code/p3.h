#include <stdio.h>


double **create_lower_from_file(const char* filename, int *cols, int *rows);
double *solve_lower(double **L, double *b, int dim);
void print_lower_solution(double **L, double *x, double *b, int rows, int cols);

