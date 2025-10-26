#pragma once

#include <concepts>
#include <cstdlib>
#include <stdexcept>
#include <vector>

namespace interp {

template <std::floating_point T>

T neville(T z, const std::vector<T> &x, const std::vector<T> &fx) {
  const std::size_t n = x.size();
  if (n != fx.size()) {
    throw std::invalid_argument("x and fx must be of the same size");
  }

  std::vector<T> Q = fx;
  for (std::size_t j = 1; j < n; j++) {
    for (std::size_t i = n - 1; i >= j; i--) {
      Q[i] =
          ((z - x[i - j]) * Q[i] - (z - x[i]) * Q[i - 1]) / (x[i] - x[i - j]);
    }
  }
  return Q[n - 1];
}

} // namespace interp
