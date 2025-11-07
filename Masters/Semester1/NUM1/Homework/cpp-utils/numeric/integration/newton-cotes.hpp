#include <array>
#include <concepts>
#include <functional>
#include <stdexcept>
#include <vector>

namespace integration {
// allows float, double and long double
namespace newtoncotes {
namespace closed {
// n = 1
template <std::floating_point T>
T trapezoid(std::array<T, 2> x, std::function<T()> f) {
  return (x[1] - x[0]) / 2.0 * (f(x[0]) + f(x[1]));
}

// n = 2
template <std::floating_point T>
T simpson(std::array<T, 3> x, std::function<T()> f) {
  return (x[2] - x[0]) / 3.0 * (f(x[0]) + 4 * f(x[1]) + f(x[2]));
}

// n = 3
template <std::floating_point T>
T simpson38(std::array<T, 4> x, std::function<T()> f) {
  return 0.375 * (x[3] - x[0]) *
         (f(x[0]) + 3 * f(x[1]) + 3 * f(x[2]) + f(x[3]));
}

// n = 4
template <std::floating_point T>
T five_point(std::array<T, 5> x, std::function<T()> f) {
  return 0.444 * (x[4] - x[0]) *
         (7 * f(x[0]) + 32 * f(x[1]) + 12 * f(x[2]) + 32 * f(x[3]) +
          7 * f(x[4]));
}

} // namespace closed

namespace open {
// n = 1
template <std::floating_point T>
T trapezoid(std::array<T, 2> x, std::function<T()> f) {
  T h = (x[1] - x[0]) / 2.0;
  return 2.0 * h * f(h);
}

// n = 2
template <std::floating_point T>
T simpson(std::array<T, 3> x, std::function<T()> f) {
  T h = (x[1] - x[0]) / 3.0;
  return 1.5 * (f(x[0]) + f(x[1]));
}

// n = 3
template <std::floating_point T>
T simpson38(std::array<T, 4> x, std::function<T()> f) {
  return 1.333 *
}

// n = 4
template <std::floating_point T>
T five_point(std::array<T, 5> x, std::function<T()> f) {
  return 0.444 * (x[4] - x[0]) *
         (7 * f(x[0]) + 32 * f(x[1]) + 12 * f(x[2]) + 32 * f(x[3]) +
          7 * f(x[4]));
}

} // namespace open

} // namespace newtoncotes

} // namespace integration
