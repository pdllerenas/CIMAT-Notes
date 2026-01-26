#include <array>
#include <concepts>
#include <stdexcept>

constexpr std::array<double, 2> LEGENDRE_2_ROOTS = {0.5773502692,
                                                    -0.5773502692};
constexpr std::array<double, 2> LEGENDRE_2_COEFF = {1.0, 1.0};

constexpr std::array<double, 3> LEGENDRE_3_ROOTS = {0.7745966692, 0,
                                                    -0.7745966692};
constexpr std::array<double, 3> LEGENDRE_3_COEFF = {0.5555555556, 0.8888888889,
                                                    0.5555555556};

constexpr std::array<double, 4> LEGENDRE_4_ROOTS = {
    0.8611363116, 0.3399810436, -0.3399810436, -0.8611363116};
constexpr std::array<double, 4> LEGENDRE_4_COEFF = {0.3478548451, 0.6521451549,
                                                    0.6521451549, 0.3478548451};

constexpr std::array<double, 5> LEGENDRE_5_ROOTS = {
    0.9061798459, 0.5384693101, 0.0, -0.5384693101, -0.9061798459};
constexpr std::array<double, 5> LEGENDRE_5_COEFF = {
    0.2369268850, 0.4786286705, 0.5688888889, 0.4786286705, 0.2369268850};

namespace integration {
namespace gaussian {

template <std::floating_point T, typename F>
static T quadrature2(T a, T b, F f) {
  T c1 = (b - a) / 2;
  T c2 = (b + a) / 2;

  return c1 * (LEGENDRE_2_COEFF[0] * f(c1 * LEGENDRE_2_ROOTS[0] + c2) +
               LEGENDRE_2_COEFF[1] * f(c1 * LEGENDRE_2_ROOTS[1] + c2));
}

template <std::floating_point T, typename F>
static T quadrature3(T a, T b, F f) {
  T c1 = (b - a) / 2;
  T c2 = (b + a) / 2;

  return c1 * (LEGENDRE_3_COEFF[0] * f(c1 * LEGENDRE_3_ROOTS[0] + c2) +
               LEGENDRE_3_COEFF[1] * f(c1 * LEGENDRE_3_ROOTS[1] + c2) +
               LEGENDRE_3_COEFF[2] * f(c1 * LEGENDRE_3_ROOTS[2] + c2));
}
template <std::floating_point T, typename F>
static T quadrature4(T a, T b, F f) {
  T c1 = (b - a) / 2;
  T c2 = (b + a) / 2;

  return c1 * (LEGENDRE_4_COEFF[0] * f(c1 * LEGENDRE_4_ROOTS[0] + c2) +
               LEGENDRE_4_COEFF[1] * f(c1 * LEGENDRE_4_ROOTS[1] + c2) +
               LEGENDRE_4_COEFF[2] * f(c1 * LEGENDRE_4_ROOTS[2] + c2) +
               LEGENDRE_4_COEFF[3] * f(c1 * LEGENDRE_4_ROOTS[3] + c2));
}
template <std::floating_point T, typename F>
static T quadrature5(T a, T b, F f) {
  T c1 = (b - a) / 2;
  T c2 = (b + a) / 2;

  return c1 * (LEGENDRE_5_COEFF[0] * f(c1 * LEGENDRE_5_ROOTS[0] + c2) +
               LEGENDRE_5_COEFF[1] * f(c1 * LEGENDRE_5_ROOTS[1] + c2) +
               LEGENDRE_5_COEFF[2] * f(c1 * LEGENDRE_5_ROOTS[2] + c2) +
               LEGENDRE_5_COEFF[3] * f(c1 * LEGENDRE_5_ROOTS[3] + c2) +
               LEGENDRE_5_COEFF[4] * f(c1 * LEGENDRE_5_ROOTS[4] + c2));
} /*
   * @brief Aproximates the integral of f from a to b
   *
   * @tparam T a floating point accuracy type (float, double, long double)
   * @param a left endpoint of the integral
   * @param b right endpoint of the integral
   * @param f function to be integrated
   *
   * @return approximation of the integral of f from a to b
   */
template <std::floating_point T, typename F>
T quadrature(T a, T b, F f, std::size_t n) {
  if (n == 2) {
    return quadrature2(a, b, f);
  } else if (n == 3) {
    return quadrature3(a, b, f);
  } else if (n == 4) {
    return quadrature4(a, b, f);
  } else if (n == 5) {
    return quadrature5(a, b, f);
  } else {
    throw std::invalid_argument("invalid polynomial degree for quadrature");
  }
}

} // namespace gaussian

} // namespace integration
