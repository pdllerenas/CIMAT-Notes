#include "../../misc/matrix.hpp"
#include <cmath>
#include <concepts>
#include <stdexcept>

namespace factorization {
namespace cholesky {

template <std::floating_point T> matrix<T> decompose(const matrix<T> &A) {
  size_t n = A.size();
  matrix<T> L(n, n, static_cast<T>(0));

  const T eps = 1e-12;

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      if (std::abs(A(i, j) - A(i, j)) > eps) {
        throw std::runtime_error("matrix is not symmetrix");
      }
    }
  }

  for (size_t j = 0; j < n; ++j) {
    T sum = 0;
    for (size_t k = 0; k < j; k++) {
      sum += L(j, k) * L(j, k);
    }

    T l_diag = A(j, j) - sum;
    if (l_diag < -eps) {
      throw std::runtime_error("matrix is not positive-definite");
    }
    L(j, j) = std::sqrt(std::max(l_diag, static_cast<T>(0)));

    for (size_t i = j + 1; i < n; ++i) {
      sum = 0;
      for (size_t k = 0; k < j; k++) {
        sum += L(i, k) * L(j, k);
      }
      L(i, j) = (A(i, j) - sum) / L(j, j);
    }
  }
  return L;
}

} // namespace cholesky

} // namespace factorization
