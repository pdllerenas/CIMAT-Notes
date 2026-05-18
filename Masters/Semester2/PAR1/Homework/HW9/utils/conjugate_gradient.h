#ifndef CG_H
#define CG_H
double *conjugate_gradient(double *A, double *b, double *x0, int N, double TOL, int MAX_ITER);
double *conjugate_gradient_sparse(int *I, int *J, double *V, int nnz, int N, double *x0, double *b, double TOL, int MAX_ITER);
#endif