#pragma once

#include <concepts>
#include <cstdlib>

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
 *
 * @note The memory allocted by this function must be freed by the caller.
 */
template <std::floating_point T>

T divided_difference(T *x, T *fx, std::size_t n) {
  T **F = malloc(n * sizeof(T *));
  for (int i = 0; i < n; i++) {
    F[i] = malloc((i + 1) * sizeof(T));
    for (int j = 0; j <= i; j++) {
      if (j == 0) {
        F[i][j] = fx[i];
      } else {
        F[i][j] = (F[i][j - 1] - F[i - 1][j - 1]) / (x[i] - x[i - j]);
      }
    }
  }

  // extract diagonal elements
  T *diag_F = malloc(n * sizeof(T));
  for (int i = 0; i < n; i++) {
    diag_F = F[i][i];
    free(F[i]);
  }
  free(F);
  return F;
}

} // namespace interp
