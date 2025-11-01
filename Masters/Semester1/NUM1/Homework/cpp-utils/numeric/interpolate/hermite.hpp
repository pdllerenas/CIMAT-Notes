#include "../../misc/matrix.hpp"
#include <concepts>
#include <stdexcept>
#include <vector>

namespace interp {
// allows float, double and long double
namespace hermite {
template <std::floating_point T>
std::vector<T> polynomial(const std::vector<T> &x, const std::vector<T> &fx,
                       const std::vector<T> &dfx) {
  std::size_t n = x.size();
  if (n != fx.size() || n != dfx.size()) {
    throw std::invalid_argument("x and fx or dfx have different sizes");
  }

  std::size_t m = 2 * n;

  std::vector<T> z(m);
  matrix<T> Q(m, m, 0.0);
  for (std::size_t i = 0; i < n; i++) {
    z[2 * i] = x[i];
    z[2 * i + 1] = x[i];

    Q(2 * i, 0) = fx[i];
    Q(2 * i + 1, 0) = fx[i];
    Q(2 * i + 1, 1) = dfx[i];

    if (i != 0) {
      Q(2 * i, 1) = (Q(2 * i, 0) - Q(2 * i - 1, 0)) / (z[2 * i] - z[2 * i - 1]);
    }
  }

  for (std::size_t i = 2; i < 2 * n; i++) {
    for (std::size_t j = 2; j <= i; j++) {
      Q(i, j) = (Q(i, j - 1) - Q(i - 1, j - 1)) / (z[i] - z[i - j]);
    }
  }
  std::vector<T> result(m);
  for (std::size_t i = 0; i < 2 * n; i++) {
    result[i] = Q(i, i);
  }
  return result;
}

template <std::floating_point T>
T eval(T z, const std::vector<T> &x, const std::vector<T> &coeff) {
  T result = coeff[0];
  T term = 1.0;
  for (std::size_t i = 1; i < coeff.size(); i++) {
    term *= (z - x[(i - 1) / 2]);
		result += coeff[i] * term;
  }
	return result;
}
} // namespace hermite

} // namespace interp
