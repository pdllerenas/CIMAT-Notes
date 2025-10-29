#pragma once

#include <concepts>
#include <cstdlib>
#include <iostream>

namespace interp {
/*
 *
 *
 *
 */
template <std::floating_point T>

T **neville(T z, T *x, T *fx, std::size_t n) {
  T **Q = (T **)malloc(n * sizeof(T *));
  for (int i = 0; i < n; i++) {
    Q[i] = (T *)malloc((i + 1) * sizeof(T));
    for (int j = 0; j <= i; j++) {
      if (j == 0) {
        Q[i][j] = fx[i];
        continue;
      }
      Q[i][j] = ((z - x[i - j]) * Q[i][j - 1] - (z - x[i]) * Q[i - 1][j - 1]) /
                (x[i] - x[i - j]);
    }
  }
  std::cout << "done" << std::endl;
  return Q;
}

} // namespace interp
