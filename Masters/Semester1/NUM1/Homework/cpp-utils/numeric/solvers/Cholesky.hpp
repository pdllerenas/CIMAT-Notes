#pragma once

#include "../../misc/matrix.hpp"
#include <cmath>
#include <concepts>
#include <stdexcept>
#include <vector>

namespace solvers {
namespace cholesky {

template <std::floating_point T>
std::vector<T> solve(const matrix<T> &A, const std::vector<T> &b) {
  // A must be square
  std::size_t n = A.rows();
  if (n != A.cols())
    throw std::invalid_argument("solve: matrix A must be square");

  if (b.size() != n)
    throw std::invalid_argument("solve: size of b does not match A.rows()");

  const T eps = static_cast<T>(1e-12);

  // Quick symmetry check (only upper triangle vs lower)
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = i + 1; j < n; ++j) {
      if (std::abs(A(i, j) - A(j, i)) > eps) {
        throw std::runtime_error(
            "solve: matrix is not symmetric (within tolerance)");
      }
    }
  }

  // Allocate lower-triangular L (n x n), init to zero
  matrix<T> L(n, n, static_cast<T>(0));

  // Cholesky decomposition: A = L * L^T
  for (std::size_t j = 0; j < n; ++j) {
    // Compute diagonal element L(j,j)
    T sum_diag = static_cast<T>(0);
    for (std::size_t k = 0; k < j; ++k) {
      sum_diag += L(j, k) * L(j, k);
    }
    T diag = A(j, j) - sum_diag;
    if (diag < -eps) {
      throw std::runtime_error("solve: matrix is not positive-definite");
    }
    diag = std::max(diag, static_cast<T>(0));
    T Ljj = std::sqrt(diag);
    if (Ljj <= eps) {
      throw std::runtime_error(
          "solve: non-positive pivot encountered (matrix may be singular)");
    }
    L(j, j) = Ljj;

    // Compute column j below diagonal: L(i,j) for i = j+1..n-1
    for (std::size_t i = j + 1; i < n; ++i) {
      T sum_off = static_cast<T>(0);
      for (std::size_t k = 0; k < j; ++k) {
        sum_off += L(i, k) * L(j, k);
      }
      L(i, j) = (A(i, j) - sum_off) / Ljj;
    }
  }

  // Forward substitution: L * y = b
  std::vector<T> y(n);
  for (std::size_t i = 0; i < n; ++i) {
    T s = static_cast<T>(0);
    for (std::size_t k = 0; k < i; ++k)
      s += L(i, k) * y[k];
    T Lii = L(i, i);
    if (std::abs(Lii) <= eps)
      throw std::runtime_error(
          "solve: zero diagonal in L during forward solve");
    y[i] = (b[i] - s) / Lii;
  }

  // Back substitution: L^T * x = y
  std::vector<T> x(n);
  for (std::size_t ii = 0; ii < n; ++ii)
    x[ii] = static_cast<T>(0);
  for (std::size_t i = n; i-- > 0;) {
    T s = static_cast<T>(0);
    for (std::size_t k = i + 1; k < n; ++k)
      s += L(k, i) * x[k];
    T Lii = L(i, i);
    if (std::abs(Lii) <= eps)
      throw std::runtime_error(
          "solve: zero diagonal in L during backward solve");
    x[i] = (y[i] - s) / Lii;
  }

  return x;
}
} // namespace cholesky

} // namespace solvers
