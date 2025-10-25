#pragma once

#include <concepts>
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
 * @return The approximate value of f at z.
 */
template <std::floating_point T>
T taylor(T z, T x0, T *df, std::size_t n) {
  T result = df[0];
  T term = df[0];
  for (int i = 1; i < n; i++) {
    term *= (z - x0) / i;
    result += df[i] * term;
  }
  return result;
}

template <std::floating_point T>
T taylor(T z, T x0, std::vector<T> df, std::size_t n) {
  T result = df[0];
  T term = df[0];
  for (std::size_t i = 1; i < n; i++) {
    term *= (z - x0) / i;
    result += df[i] * term;
  }
  return result;
}

} // namespace interp
