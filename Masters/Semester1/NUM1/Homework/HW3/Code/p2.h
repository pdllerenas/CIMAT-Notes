#include <stdio.h>


double **create_upper_from_file(FILE *file, int *cols, int *rows);
double *solve_upper(double **U, double *b, int dim);
void print_upper_solution(double **U, double *b, double *x, int rows, int cols);
