#pragma once

#include "../../misc/matrix.hpp"
#include <cmath>
#include <concepts>
#include <vector>

namespace differential {

/*
 * @brief Given a function f:R^n -> R^m, approximate the Jacobian matrix of f.
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @tparam F std::function
 * @param f vector of std::functions, f:R^n -> R^m
 * @param x evaluation point
 * @param h finite difference step
 *
 * @return Jacobian matrix of f at x
 */
template <std::floating_point T, typename F>
matrix<T> jacobian(const std::vector<F> &f, const std::vector<T> &x,
                   T h = 1e-6) {
  size_t n = x.size();
  size_t m = f.size();
  matrix<T> J(m, n, 0);

  std::vector<T> xh = x;

  for (size_t j = 0; j < n; ++j) {
    T orig = x[j];
    xh[j] = orig + h;

    std::vector<T> fp(m);
    for (size_t i = 0; i < m; ++i) {
      fp[i] = f[i](xh);
    }

    std::vector<T> f0(m);
    for (size_t i = 0; i < m; ++i) {
      f0[i] = f[i](x);
    }

    for (size_t i = 0; i < m; ++i) {
      J[i][j] = (fp[i] - f0[i]) / h;
    }

    xh[j] = orig;
  }
  return J;
}

template <std::floating_point T, typename Function>
matrix<T> jacobian(Function f, const std::vector<T> &x, T h = 1e-6) {
  std::vector<T> fx = f(x);
  size_t m = fx.size();
  size_t n = x.size();
  matrix<T> J(m, n);

  for (size_t j = 0; j < n; ++j) {
    std::vector<T> xh = x;
    xh[j] += h;
    std::vector<T> fxh = f(xh);
    for (size_t i = 0; i < m; ++i) {
      J(i, j) = (fxh[i] - fx[i]) / h;
    }
  }
  return J;
}
} // namespace differential
