#pragma once

#include "../../misc/matrix.hpp"
#include "../../numeric/differential/Jacobian.hpp"
#include <cmath>
#include <concepts>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace solvers {
namespace broyden {

template <std::floating_point T, typename Func>
std::vector<T> solve(const std::vector<Func> &F, std::vector<T> x, T TOL,
                     size_t MAX_ITER, std::ostream *out = nullptr) {
  const size_t n = F.size();

  matrix<T> J = differential::jacobian(F, x);
  matrix<T> A = J.inverse();

  if (out) {
    for (size_t i = 0; i < n - 1; i++) {
      *out << 'x' << i << ',';
    }
    *out << 'x' << n - 1 << ',';
    *out << "||x_prev - x||\n";
  }

  // v = F(x)
  std::vector<T> v(n);
  for (size_t i = 0; i < n; i++) {
    v[i] = F[i](x);
  }

  std::vector<T> s = -1.0 * (A * v);
  x = x + s;
  size_t k = 2;

  while (k <= MAX_ITER) {
    std::vector<T> w = v;

    for (size_t i = 0; i < n; ++i)
      v[i] = F[i](x);

    std::vector<T> y = v - w;

    std::vector<T> z = -1.0 * (A * y);

    T p = -std::inner_product(s.begin(), s.end(), z.begin(), T{});

    // recompute jacobian if s and z are orthogonal or nearly orthogonal
    if (std::abs(p) < 1e-14) {
      matrix<T> J = differential::jacobian(F, x);
      A = J.inverse();
      s = -1.0 * (A * v);
      ++k;
      continue;
    }

    std::vector<T> u = A.transpose() * s;

    A = A + ((T(1) / p) * outer_product(s + z, u));

    s = -1.0 * (A * v);

    x = x + s;

    if (out) {
      for (size_t i = 0; i < n; i++) {
        *out << x[i] << ',';
      }
      *out << norm(s) << '\n';
    }

    if (norm(s) < TOL)
      return x;

    ++k;
  }
  throw std::runtime_error(
      "maximum iterations reached without convergence in broyden");
}
} // namespace broyden

} // namespace solvers
