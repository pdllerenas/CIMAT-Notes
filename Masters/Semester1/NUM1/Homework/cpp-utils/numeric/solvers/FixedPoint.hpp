#pragma once

#include "../../misc/matrix.hpp"
#include <cmath>
#include <concepts>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace solvers {
namespace fixed_point {

template <std::floating_point T, typename Func>
std::vector<T> solve(const std::vector<Func> &F, std::vector<T> x, T TOL,
                     size_t MAX_ITER, std::ostream *out = nullptr) {
  size_t k = 1;
  size_t n = F.size();

  std::vector<T> x_prev = x;
  if (out) {
    for (size_t i = 0; i < n - 1; i++) {
      *out << 'x' << i << ',';
    }
    *out << 'x' << n - 1 << ',';
    *out << "||x_prev - x||\n";
  }

  while (k <= MAX_ITER) {
    for (size_t i = 0; i < n; i++) {
      x[i] = F[i](x);
      if (out) {
        *out << x[i] << ',';
      }
    }

    T err = norm(x_prev - x);
    if (out)
      *out << err << '\n';

    if (err < TOL)
      return x;

    x_prev = x;
    k++;
  }

  throw std::runtime_error("maximum iterations reached without convergence in "
                           "fixed point iteration");
}
} // namespace fixed_point

} // namespace solvers
