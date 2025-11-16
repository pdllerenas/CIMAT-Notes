#pragma once

#include "../../misc/matrix.hpp"
#include <cmath>
#include <concepts>
#include <stdexcept>
#include <vector>

namespace solvers {
namespace iterative {

template <std::floating_point T>
std::vector<T> jacobi(const matrix<T> &A, const std::vector<T> &b,
                      std::vector<T> x0, T TOL, size_t MAX_ITER) {
  // A must be square
  std::size_t n = A.rows();
  if (n != A.cols())
    throw std::invalid_argument("matrix A must be square");

  if (b.size() != n)
    throw std::invalid_argument("size of b does not match A.rows()");

  size_t k = 1;
  std::vector<T> x;

  while (k < MAX_ITER) {
    for (size_t i = 1; i < n; i++) {
      T sum = 0.0;
      for (size_t k = 0; k < n; k++) {
        if (k == i) {
          continue;
        }
        sum += A(i, k) * x[k];
      }
      x[i] = 1 / A(i, i) * (-sum) + b[i];
    }
  }

  k++;
  for (size_t i = 0; i < n; ++i) {
    x0[i] = x[i];
  }

  return x;
}

template <std::floating_point T>
std::vector<T> gauss_siedel(const matrix<T> &A, const std::vector<T> &b,
                            std::vector<T> x0, T TOL, size_t MAX_ITER) {
  // A must be square
  std::size_t n = A.rows();
  if (n != A.cols())
    throw std::invalid_argument("matrix A must be square");

  if (b.size() != n)
    throw std::invalid_argument("size of b does not match A.rows()");

  size_t k = 1;
  std::vector<T> x;

  while (k < MAX_ITER) {
    for (size_t i = 1; i < n; ++i) {
      T sum = 0.0;
      for (size_t j = 0; j < i; ++j) {
        sum += A(i, j) * x[j];
      }

      for (size_t j = i + 1; j < n; ++j) {
        sum += A(i, j) * x0[j];
      }

      x[i] = b[i] - (sum / A(i, i));
    }
  }

  k++;
  for (size_t i = 0; i < n; ++i) {
    x0[i] = x[i];
  }

  return x;
}
} // namespace iterative

} // namespace solvers
