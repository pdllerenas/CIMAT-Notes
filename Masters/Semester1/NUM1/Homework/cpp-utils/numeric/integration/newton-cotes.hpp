#include <concepts>

namespace integration {
namespace newtoncotes {
namespace closed {

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */

// n = 1 (Trapezoidal rule)
template <std::floating_point T, typename F>
T trapezoid(T a, T b, F f) {
  return (b - a) / 2.0 * (f(a) + f(b));
}

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 2 (Simpson's 1/3 rule)
template <std::floating_point T, typename F>
T simpson(T a, T b, F f) {
  T h = (b - a) / 2.0;
  return (b - a) / 6.0 * (f(a) + 4 * f(a + h) + f(b));
}

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 3 (Simpson’s 3/8 rule)
template <std::floating_point T, typename F>
T simpson38(T a, T b, F f) {
  T h = (b - a) / 3.0;
  return 3.0 * (b - a) / 8.0 * (f(a) + 3 * f(a + h) + 3 * f(a + 2 * h) + f(b));
}

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 4 (Boole’s / five-point rule)
template <std::floating_point T, typename F>
T five_point(T a, T b, F f) {
  T h = (b - a) / 4.0;
  return (b - a) / 90.0 *
         (7 * f(a) + 32 * f(a + h) + 12 * f(a + 2 * h) + 32 * f(a + 3 * h) +
          7 * f(b));
}

} // namespace closed

namespace open {
/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 0 (midpoint rule)
template <std::floating_point T, typename F>
T midpoint(T a, T b, F f) {
  return 2.0 * (b - a) * f((a + b) / 2.0);
}

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 1
template <std::floating_point T, typename F>
T n2(T a, T b, F f) {
  T h = (b - a) / 3.0;
  return (h / 2.0) * (f(a + h) + f(a + 2 * h));
}

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 2 (Milne's rule)
template <std::floating_point T, typename F>
T simpson38(T a, T b, F f) {
  T h = (b - a) / 4.0;
  return (h / 3.0) * (2 * f(a + h) - f(a + 2 * h) + 2 * f(a + 3 * h));
}

/*
 * @brief Aproximates the integral of f from a to b
 *
 * @tparam T a floating point accuracy type (float, double, long double)
 * @param a left endpoint of the integral
 * @param b right endpoint of the integral
 * @param f function to be integrated
 *
 * @return approximation of the integral of f from a to b
 */
// n = 3
template <std::floating_point T, typename F>
T five_point(T a, T b, F f) {
  T h = (b - a) / 5.0;
  return (h / 24.0) *
         (11 * f(a + h) + f(a + 2 * h) + f(a + 3 * h) + 11 * f(a + 4 * h));
}

} // namespace open

} // namespace newtoncotes

} // namespace integration
