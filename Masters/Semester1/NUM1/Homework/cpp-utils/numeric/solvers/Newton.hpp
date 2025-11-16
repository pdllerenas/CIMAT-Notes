#pragma once

#include "../../misc/matrix.hpp"
#include "../../numeric/differential/Jacobian.hpp"
#include "../../numeric/solvers/Crout.hpp"
#include <cmath>
#include <concepts>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace solvers {
namespace newton {

template <std::floating_point T, typename Func>
std::vector<T> solve(const std::vector<Func> &F, std::vector<T> x, T TOL,
                     size_t MAX_ITER) {
  const size_t n = F.size();
  size_t k = 1;

  while (k <= MAX_ITER) {
    // v = F(x)
    std::vector<T> v(n);
    for (size_t i = 0; i < n; i++) {
      v[i] = F[i](x);
    }
    matrix<T> J = differential::jacobian(F, x);

    matrix<T> LU = J;
    std::vector<size_t> pivots;
    solvers::crout::factorize(LU, pivots);

    std::vector<T> s = solvers::crout::solve(LU, pivots, v);

    for (size_t i = 0; i < n; i++) {
      x[i] -= s[i];
    }

    if (norm(s) < TOL) {
      return x;
    }
  }
  throw std::runtime_error(
      "maximum iterations reached without convergence in newton");
}
} // namespace newton

} // namespace solvers
