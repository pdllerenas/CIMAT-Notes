#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "matrix_factorization.h"
#include "matrix_io.h"
#include "matrix_operations.h"
#include "vector.h"

/*

L_{ii} = sqrt(A_{jj} - sum_{k = 1}^{j-1} L^2_{jk})
L_{ij} = 1/L_{jj}(A_{ij} - sum_{k=1}^{j-1}L_{ik}L_{jk})

The Cholesky method works only for symmetric positive definite matrices,
and the decomposition is of the form A = LL^T.

 */
void cholesky(const Matrix *A, Matrix *L, int n) {
  int i, j, k;
  double sum;

  for (j = 0; j < n; j++) {
    sum = 0.0;
    for (k = 0; k < j; k++) {
      sum += ((double *)L->data)[j * n + k] * ((double *)L->data)[j * n + k];
    }

    double l_diag = ((double *)A->data)[j * n + j] - sum;
    if (l_diag <= 0) {
      fprintf(stderr, "Matrix A is not positive-definite.\n");
      return;
    }

    ((double *)L->data)[j * n + j] = sqrt(l_diag);

    for (i = j + 1; i < n; i++) {
      sum = 0.0;
      for (k = 0; k < j; k++) {
        sum += ((double *)L->data)[i * n + k] * ((double *)L->data)[j * n + k];
      }
      ((double *)L->data)[i * n + j] = (((double *)A->data)[i * n + j] - sum) /
                                       ((double *)L->data)[j * n + j];
    }
  }
}

/*

we assume A has the form of a symmetric tridiagonal matrix, that is,
the b main diagonals are the only non-zero values.
We access the values of A by assuming it has only 2 columns

 */
void cholesky_symmetric_banded(const Matrix *A, Matrix *L) {
  int i;
  double l_diag, m_off_diag;

  // the first diagonal term does not need the sum (due to the tridiagonal
  // hypothesis)
  l_diag = ((double *)A->data)[0];
  if (l_diag <= 0) {
    fprintf(stderr, "Matrix A is not positive-definite.\n");
    return;
  }

  ((double *)L->data)[0] = sqrt(l_diag);

  ((double *)L->data)[L->cols] =
      ((double *)A->data)[1] / ((double *)L->data)[0];

  // rest of the terms
  for (i = 1; i < A->rows; i++) {
    // since it is tridiagonal, the only non-zero term on the sum is the one
    // before the diagonal, which incidentally, is also the last term in the sum
    m_off_diag = ((double *)L->data)[i * L->cols + (i - 1)];

    l_diag = ((double *)A->data)[i * 2] - (m_off_diag * m_off_diag);
    if (l_diag <= 0) {
      fprintf(stderr, "Matrix A is not positive-definite.\n");
      return;
    }

    ((double *)L->data)[i * L->cols + i] = sqrt(l_diag);

    // the non-diagonal terms of L have a zero sum, so we only have
    // L[i][j] = A[i][j]/L[j][j]. Since only A[j+1][j] is non-zero (for the
    // lower terms), we only calculate for this specific value
    if (i < A->rows - 1) {
      m_off_diag = ((double *)A->data)[i * 2 + 1];
      ((double *)L->data)[(i + 1) * L->cols + i] =
          m_off_diag / ((double *)L->data)[i * L->cols + i];
    }
  }
}

/*

decomposition of A = LU, where L has diagonal equal to only 1's.

 */

void doolittle(const Matrix *A, Matrix *L, Matrix *U, int n) {
  int i, j, k;
  double sum;

  for (i = 0; i < n; i++) {
    ((double *)L->data)[i * n + i] = 1.0;
  }

  for (j = 0; j < n; j++) {
    for (i = 0; i <= j; i++) {
      sum = 0.0;
      for (k = 0; k < i; k++) {
        sum += ((double *)L->data)[i * n + k] * ((double *)U->data)[k * n + j];
      }
      ((double *)U->data)[i * n + j] = ((double *)A->data)[i * n + j] - sum;
    }

    for (i = j + 1; i < n; i++) {
      sum = 0.0;
      for (k = 0; k < j; k++) {
        sum += ((double *)L->data)[i * n + k] * ((double *)U->data)[k * n + j];
      }
      if (((double *)U->data)[j * n + j] == 0) {
        fprintf(stderr, "Can't divide by 0.\n");
        exit(1);
      }
      ((double *)L->data)[i * n + j] = (((double *)A->data)[i * n + j] - sum) /
                                       ((double *)U->data)[j * n + j];
    }
  }
}

/*

decomposition of A = LU, where the diagonal of U is purely 1's.

 */

void crout(const Matrix *A, Matrix *L, Matrix *U, int n) {
  int i, j, k;
  double sum;

  for (i = 0; i < n; i++) {
    ((double *)U->data)[i * n + i] = 1.0;
  }

  for (j = 0; j < n; j++) {
    for (i = j; i < n; i++) {
      sum = 0.0;
      for (k = 0; k < j; k++) {
        sum = sum +
              ((double *)L->data)[i * n + k] * ((double *)U->data)[k * n + j];
      }
      ((double *)L->data)[i * n + j] = ((double *)A->data)[i * n + j] - sum;
    }

    for (i = j; i < n; i++) {
      sum = 0.0;
      for (k = 0; k < j; k++) {
        sum += ((double *)L->data)[j * n + k] * ((double *)U->data)[k * n + i];
      }
      if (((double *)L->data)[j * n + j] == 0) {
        fprintf(stderr, "Can't divide by 0.\n");
        exit(1);
      }
      ((double *)U->data)[j * n + i] = (((double *)A->data)[j * n + i] - sum) /
                                       ((double *)L->data)[j * n + j];
    }
  }
}

/*

The Householder method takes in a symmetric matrix and converts it to a
tridiagonal matrix

 */
void householder(Matrix *A) {
  int n = A->rows;
  int j, k;
  double alpha;

  double *adata = (double *)A->data;
  double *v = calloc(n, sizeof(double));
  if (!v) {
    fprintf(stderr, "Could not allocate memory.\n");
    return;
  }

  double *u = calloc(n, sizeof(double));
  if (!u) {
    fprintf(stderr, "Could not allocate memory.\n");
    free(v);
    return;
  }

  double *z = calloc(n, sizeof(double));
  if (!z) {
    fprintf(stderr, "Could not allocate memory.\n");
    free(u);
    free(v);
    return;
  }

  for (k = 0; k < n - 1; k++) {
    double q = 0.0;
    for (j = k + 1; j < n; j++) {
      // q += a_jk^2
      q += adata[j * n + k] * adata[j * n + k];
    }
    double ak = adata[(k + 1) * n + k];
    if (ak == 0) {
      alpha = -sqrt(q);
    } else {
      alpha = -sqrt(q) * ak / fabs(ak);
    }
    // RSQ = 2r^2
    double RSQ = alpha * alpha - alpha * ak;
    v[k + 1] = ak - alpha;

    for (j = k + 2; j < n; j++) {
      v[j] = adata[j * n + k];
    }

    for (j = k; j < n; j++) {
      double uj = 0.0;
      // sum a_ji * vi
      for (int i = k + 1; i < n; i++) {
        uj += adata[j * n + i] * v[i];
      }
      u[j] = (1 / RSQ) * uj;
    }

    double PROD = 0.0;
    for (j = k + 1; j < n; j++) {
      PROD += v[j] * u[j];
    }

    for (j = k; j < n; j++) {
      z[j] = u[j] - (PROD / (2 * RSQ)) * v[j];
    }

    for (int i = k + 1; i < n; i++) {
      for (int l = i; l < n; l++) {
        if (i == l) {
          adata[i * n + l] -= 2.0 * v[i] * z[l];
        } else {
          adata[i * n + l] -= v[i] * z[l] + v[l] * z[i];
          // symmetric
          adata[l * n + i] = adata[i * n + l];
        }
      }
    }

    for (j = k + 2; j < n; j++) {
      adata[k * n + j] = adata[j * n + k] = 0;
    }

    adata[(k + 1) * n + k] -= v[k + 1] * z[k];

    adata[k * n + k + 1] = adata[(k + 1) * n + k];
  }
  free(u);
  free(v);
  free(z);
}

/*

Given a matrix A, calculate the decomposition A = QR,
where Q is an orthonormal matrix

 */
void QR_factorization(const Matrix *A, Matrix **Q_out, Matrix **R_out) {
  int n = A->rows;
  int m = A->cols;

  // minimum between m and n
  int mn_min = (m < n ? m : n);

  Matrix *Q = identity_matrix(n);
  Matrix *R = matrix_create_double(n, m);
  copy_matrix(R, A);

  for (int k = 0; k < mn_min; k++) {
    // Extract column k from row k downward
    Vector *x = create_vector(n);
    for (int i = 0; i < n; i++) {
      ((double *)x->data)[i] = (i < k) ? 0.0 : ((double *)R->data)[i * m + k];
    }

    double normx = l2_norm(x);

    // normx if xk < 0, -normx if xk > 0
    double alpha = -copysign(normx, ((double *)x->data)[k]);

    Vector *v = create_vector(n);
    for (int i = 0; i < n; i++) {
      ((double *)v->data)[i] = ((double *)x->data)[i];
    }

    ((double *)v->data)[k] -= alpha;
    double vnorm = l2_norm(v);
    vector_scalar_product_inplace(v, 1.0 / vnorm);

    // m = I - 2v v^T
    Matrix *H = vmul(v);

    Matrix *HR = matrix_product(H, R);
    matrix_free(R);
    R = HR;

    Matrix *QH = matrix_product(Q, H);
    matrix_free(Q);
    Q = QH;

    free_vector(x);
    free_vector(v);
    matrix_free(H);
  }

  *Q_out = Q;
  *R_out = R;
}
