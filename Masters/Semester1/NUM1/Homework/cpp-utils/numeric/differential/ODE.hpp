#pragma once

#include <cmath>
#include <concepts>
#include <vector>

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
std::vector<std::tuple<T, T>> rk4(F f, T a, T b, size_t N, T alpha) {
  std::vector<std::tuple<T, T>> result;
  T h = (b - a) / N;
  T t = a;
  T w = alpha;
	result.emplace_back(t,w);

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
} // namespace differential
