#include "matrix.h"
#include "vector.h"

double iterative_power(const Matrix *, const Vector *, const double, const int,
                       Vector **);
double inverse_iterative_power(Matrix *, Vector *, double, int);
void get_m_largest_eigenvalues(const Matrix *, const Vector *, unsigned int,
                               double, int, double *, Vector **);
