#pragma once

#include <concepts>
#include <cstdlib>
#include <stdexcept>
#include <vector>

namespace interp {
/*
 * @brief Interpolates the value of a function at the value z
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param z point to interpolate.
 * @param x0 center of taylor expansion.
 * @param *df pointer to derivatives of f.
 * @param n size of derivative array.
 *
 * @return coefficients of the interpolating polynomial
 */
template <std::floating_point T>

std::vector<T> divided_difference(const std::vector<T> &x,
                                  const std::vector<T> &fx) {
  std::size_t n = x.size();
  if (n != fx.size()) {
    throw std::invalid_argument("x and fx do not have the same dimension");
  }
  std::vector<T> F = fx;
  for (std::size_t j = 1; j < n; j++) {
    for (std::size_t i = n - 1; i >= j; i--) {
      F[i] = (F[i] - F[i - 1]) / (x[i] - x[i - j]);
    }
  }

  return F;
}

template <std::floating_point T>
T evaluate(T z, const std::vector<T> &x, const std::vector<T> &coeff) {
  std::size_t n = x.size();

  if (n != coeff.size()) {
    throw std::invalid_argument("x and coeff have different sizes");
  }
  T result = coeff[n - 1];

  for (std::size_t i = n - 1; i-- > 0;) {
    result = result * (z - x[i]) + coeff[i];
  }
  return result;
}

} // namespace interp
