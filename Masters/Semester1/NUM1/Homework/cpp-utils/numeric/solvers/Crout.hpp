#pragma once

#include "../../misc/matrix.hpp"
#include <cmath>
#include <concepts>
#include <stdexcept>
#include <vector>

namespace solvers {
namespace crout {

template <std::floating_point T>
void factorize(matrix<T> &A, std::vector<size_t> &P) {
	T eps = std::numeric_limits<T>::epsilon();

  size_t n = A.rows();
  P.resize(n);
  for (size_t i = 0; i < n; ++i)
    P[i] = i;

  // pivoting
  for (size_t k = 0; k < n; ++k) {
    T max_val = 0;
    size_t pivot = k;
    for (size_t i = k; i < n; ++i) {
      if (std::abs(A(i, k)) > max_val) {
        max_val = std::abs(A(i, k));
        pivot = i;
      }
    }

    if (max_val < eps * std::abs(A(k,k)))
      throw std::runtime_error("LU factorization: singular matrix");

    if (pivot != k) {
			A.swap(k, pivot);
      std::swap(P[k], P[pivot]);
    }

    for (size_t i = k + 1; i < n; ++i) {
      A(i, k) /= A(k, k);
      for (size_t j = k + 1; j < n; ++j) {
        A(i, j) -= A(i, k) * A(k, j);
      }
    }
  }
}

template <std::floating_point T>
std::vector<T> solve(const matrix<T> &A, const std::vector<size_t> &P,
                        const std::vector<T> &b) {
  size_t n = A.rows();
  if (b.size() != n)
    throw std::runtime_error("dimension mismatch in lu_solve");

  std::vector<T> x(n);

  for (size_t i = 0; i < n; ++i) {
    x[i] = b[P[i]];
  }

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      x[i] -= A(i, j) * x[j];
    }
  }

  for (size_t i = n; i-- > 0;) {
    for (size_t j = i + 1; j < n; ++j) {
      x[i] -= A(i, j) * x[j];
    }
    x[i] /= A(i, i);
  }

  return x;
}
} // namespace crout

} // namespace solvers
