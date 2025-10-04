#include "matrix.h"
#include "vector.h"

double iterative_power(const Matrix *, const Vector *, const double, const int,
                       Vector **);
void get_m_largest_eigenvalues(const Matrix *, const Vector *, unsigned int,
                               double, int, double *, Vector **);
void get_m_smallest_eigenvalues(const Matrix *, const Vector *, unsigned int,
                                double, int, double *, Vector **);
double inverse_iterative_power(Matrix *A, Vector *x0, double TOL, int MAX_ITER,
                               Vector **v_i, Vector **prev_vecs,
                               int found_count);

double **subspace_iteration(const Matrix *A, Matrix *phi_0, double TOL,
                            int MAX_ITER);

Vector *conjugate_gradient(Matrix *, Vector *, Vector *x0, double TOL,
                           int MAX_ITER);
double matrix_conjugate_with_vector(Matrix *A, Vector *v);
Matrix *jacobi_eigenvalue(Matrix *S, double *eigenvalues, double TOL, int MAX_ITER);
