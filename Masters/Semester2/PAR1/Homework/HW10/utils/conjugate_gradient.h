#ifndef CG_H
#define CG_H
#ifdef MPI_VERSION
void conjugate_gradient_3diag_MPI(double diag, double off_diag, double *u, int local_N, double TOL, int MAX_ITER, MPI_Comm comm);
#endif
double *conjugate_gradient_3diag(double diag, double off_diag, double *x0, double *b, int N, double TOL, int MAX_ITER);
#endif