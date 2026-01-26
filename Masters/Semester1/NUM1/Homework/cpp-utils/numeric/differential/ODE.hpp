#pragma once

#include <cmath>
#include <concepts>
#include <vector>

#include "../../misc/matrix.hpp"

namespace differential {

/*
 * @brief given the differential equation y' = f(t,y) with initial value y(a) =
 * alpha, calculate the approximate value of the solution
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @tparam F std::function
 *
 * @param f function such that y' = f(t, y)
 * @param a left endpoint
 * @param b right endpoint
 * @param N steps dividing the interval [a,b]
 * @param alpha initial condition value
 *
 * @return vector<T> approximate solution to given differential equation in the
 * points [a,b] with step N.
 */
template <std::floating_point T, typename F>
std::vector<std::pair<T, T>> rk4(F f, T a, T b, size_t N, T alpha) {
  std::vector<std::pair<T, T>> result;
  result.reserve(N + 1);
  T h = (b - a) / N;
  T t = a;
  T w = alpha;
  result.emplace_back(t, w);

  for (size_t i = 1; i <= N; i++) {
    T K1 = h * f(t, w);
    T K2 = h * f(t + h / 2, w + K1 / 2);
    T K3 = h * f(t + h / 2, w + K2 / 2);
    T K4 = h * f(t + h, w + K3);

    w = w + (K1 + 2 * K2 + 2 * K3 + K4) / 6;
    t = a + i * h;
    result.emplace_back(t, w);
  }
  return result;
}

template <std::floating_point T, typename F>
matrix<T> rk4(F f, T a, T b, size_t N, const std::vector<T> &alpha) {
  size_t n = alpha.size();

  matrix<T> result(N + 1, n + 1);
  T h = (b - a) / static_cast<T>(N);

  T t = a;
  std::vector<T> y = alpha;

  result(0, 0) = t;
  for (size_t j = 0; j < n; ++j)
    result(0, j + 1) = y[j];

  for (size_t i = 1; i <= N; ++i) {

    std::vector<T> K1 = f(t, y);

    std::vector<T> y2(n);
    for (size_t j = 0; j < n; ++j)
      y2[j] = y[j] + (h / 2) * K1[j];
    std::vector<T> K2 = f(t + h / 2, y2);

    std::vector<T> y3(n);
    for (size_t j = 0; j < n; ++j)
      y3[j] = y[j] + (h / 2) * K2[j];
    std::vector<T> K3 = f(t + h / 2, y3);

    std::vector<T> y4(n);
    for (size_t j = 0; j < n; ++j)
      y4[j] = y[j] + h * K3[j];
    std::vector<T> K4 = f(t + h, y4);

    for (size_t j = 0; j < n; ++j)
      y[j] += (h / 6) * (K1[j] + 2 * K2[j] + 2 * K3[j] + K4[j]);

    t += h;

    result(i, 0) = t;
    for (size_t j = 0; j < n; ++j)
      result(i, j + 1) = y[j];
  }

  return result;
}
/*
 * @brief given the differential equation y' = f(t,y) with initial value y(a) =
 * alpha, calculate the approximate value of the solution
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @tparam F std::function
 *
 * @param f function such that y' = f(t, y)
 * @param a left endpoint
 * @param b right endpoint
 * @param N steps dividing the interval [a,b]
 * @param alpha initial condition value
 *
 * @return vector<T> approximate solution to given differential equation in the
 * points [a,b] with step N.
 */
template <std::floating_point T, typename F>
std::vector<std::pair<T, T>> euler(F f, T a, T b, size_t N, T alpha) {
  std::vector<std::pair<T, T>> result;
  result.reserve(N + 1);
  T h = (b - a) / N;
  T t = a;
  T w = alpha;
  result.emplace_back(t, w);

  for (size_t i = 1; i <= N; i++) {
    w = w + h * f(t, w);
    t = t + h;
    result.emplace_back(t, w);
  }
  return result;
}

template <std::floating_point T, typename F>
matrix<T> euler(F f, T a, T b, size_t N, const std::vector<T> &alpha) {
  size_t n = alpha.size();

  matrix<T> result(N + 1, n + 1);

  T h = (b - a) / static_cast<T>(N);
  T t = a;
  std::vector<T> y = alpha;

  result(0, 0) = t;
  for (size_t j = 0; j < n; ++j)
    result(0, j + 1) = y[j];

  for (size_t i = 1; i <= N; ++i) {
    std::vector<T> f0 = f(t, y);

    for (size_t j = 0; j < n; ++j)
      y[j] += h * f0[j];

    t += h;

    result(i, 0) = t;
    for (size_t j = 0; j < n; ++j)
      result(i, j + 1) = y[j];
  }

  return result;
}

/*
 * @brief given the differential equation y' = f(t,y) with initial value y(a) =
 * alpha, calculate the approximate value of the solution
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @tparam F std::function
 *
 * @param f function such that y' = f(t, y)
 * @param a left endpoint
 * @param b right endpoint
 * @param N steps dividing the interval [a,b]
 * @param alpha initial condition value
 *
 * @return vector<T> approximate solution to given differential equation in the
 * points [a,b] with step N.
 */
template <std::floating_point T, typename F>
std::vector<std::pair<T, T>> taylor2(F f, T a, T b, size_t N, T alpha) {
  std::vector<std::pair<T, T>> result;
  result.reserve(N + 1);

  T h = (b - a) / N;
  T t = a;
  T w = alpha;
  result.emplace_back(t, w);
  constexpr T eps = 1e-6;

  for (size_t i = 0; i < N; ++i) {
    T f0 = f(t, w);
    T ft = (f(t + eps, w) - f0) / eps; // df/dt
    T fy = (f(t, w + eps) - f0) / eps; // df/dy
    T Tn = f0 + (h / 2) * (ft + fy * f0);

    w = w + h * Tn;
    t += h;
    result.emplace_back(t, w);
  }

  return result;
}

template <std::floating_point T, typename F>
matrix<T> taylor2(F f, T a, T b, size_t N, const std::vector<T> &alpha) {
  size_t n = alpha.size();

  matrix<T> result(N + 1, n + 1);

  T h = (b - a) / static_cast<T>(N);
  T t = a;
  std::vector<T> y = alpha;

  constexpr T eps = 1e-6;

  result(0, 0) = t;
  for (size_t j = 0; j < n; ++j)
    result(0, j + 1) = y[j];

  for (size_t i = 1; i <= N; ++i) {
    std::vector<T> f0 = f(t, y);

    // --- df/dt
    std::vector<T> f_dt(n);
    std::vector<T> f_t_eps = f(t + eps, y);
    for (size_t j = 0; j < n; ++j)
      f_dt[j] = (f_t_eps[j] - f0[j]) / eps;

    std::vector<std::vector<T>> J(n, std::vector<T>(n));
    for (size_t k = 0; k < n; ++k) {
      std::vector<T> y_eps = y;
      y_eps[k] += eps;

      std::vector<T> f_eps = f(t, y_eps);
      for (size_t j = 0; j < n; ++j)
        J[j][k] = (f_eps[j] - f0[j]) / eps;
    }

    std::vector<T> Jf(n, T(0));
    for (size_t j = 0; j < n; ++j)
      for (size_t k = 0; k < n; ++k)
        Jf[j] += J[j][k] * f0[k];

    std::vector<T> f1(n);
    for (size_t j = 0; j < n; ++j)
      f1[j] = f_dt[j] + Jf[j];

    for (size_t j = 0; j < n; ++j)
      y[j] += h * f0[j] + (h * h / 2) * f1[j];

    t += h;

    result(i, 0) = t;
    for (size_t j = 0; j < n; ++j)
      result(i, j + 1) = y[j];
  }

  return result;
}
/*
 * @brief given the differential equation y' = f(t,y) with initial value y(a) =
 * alpha, calculate the approximate value of the solution
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @tparam F std::function
 *
 * @param f function such that y' = f(t, y)
 * @param a left endpoint
 * @param b right endpoint
 * @param N steps dividing the interval [a,b]
 * @param alpha initial condition value
 *
 * @return vector<T> approximate solution to given differential equation in the
 * points [a,b] with step N.
 */
template <std::floating_point T, typename F>
std::vector<std::pair<T, T>> heun(F f, T a, T b, size_t N, T alpha) {
  std::vector<std::pair<T, T>> result;
  result.reserve(N + 1);
  T h = (b - a) / N;
  T t = a;
  T w = alpha;
  result.emplace_back(t, w);

  for (size_t i = 0; i < N; ++i) {
    T k1 = f(t, w);
    T w_predict = w + h * k1;
    T k2 = f(t + h, w_predict);

    w = w + (h / 2) * (k1 + k2);
    t = t + h;

    result.emplace_back(t, w);
  }

  return result;
}

template <std::floating_point T, typename F>
matrix<T> heun(F f, T a, T b, size_t N, const std::vector<T> &alpha) {
  size_t n = alpha.size();

  matrix<T> result(N + 1, n + 1);

  T h = (b - a) / static_cast<T>(N);
  T t = a;

  std::vector<T> w = alpha;

  result(0, 0) = t;
  for (size_t j = 0; j < n; ++j) {
    result(0, j + 1) = w[j];
  }

  for (size_t i = 1; i <= N; ++i) {
    std::vector<T> K1 = f(t, w);

    std::vector<T> w_tilde(n);
    for (size_t j = 0; j < n; ++j)
      w_tilde[j] = w[j] + h * K1[j];

    std::vector<T> K2 = f(t + h, w_tilde);

    for (size_t j = 0; j < n; ++j)
      w[j] += (h / 2) * (K1[j] + K2[j]);

    t += h;

    result(i, 0) = t;
    for (size_t j = 0; j < n; ++j) {
      result(i, j + 1) = w[j];
    }
  }

  return result;
}
} // namespace differential
