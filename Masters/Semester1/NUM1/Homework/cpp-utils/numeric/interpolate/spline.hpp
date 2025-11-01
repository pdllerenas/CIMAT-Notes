#include <concepts>
#include <stdexcept>
#include <vector>

namespace interp {
namespace spline {
// allows float, double and long double
template <std::floating_point T>
std::vector<std::tuple<T, T, T, T>> natural(const std::vector<T> &x,
                                            const std::vector<T> &fx) {
  std::size_t n = x.size();
  if (n != fx.size()) {
    throw std::invalid_argument("x and fx or dfx have different sizes");
  }

  std::vector<T> h(n - 1);
  for (std::size_t i = 0; i < n - 1; i++) {
    h[i] = x[i + 1] - x[i];
  }

  std::vector<T> al(n - 1);
  for (std::size_t i = 1; i < n - 1; i++) {
    al[i] =
        3.0 * (fx[i + 1] - fx[i]) / h[i] - 3.0 * (fx[i] - fx[i - 1]) / h[i - 1];
  }

  // solve tridiagonal system
  std::vector<T> l(n, 0), mu(n, 0), z(n, 0);

  l[0] = 1;
  mu[0] = 0;
  z[0] = 0;

  for (std::size_t i = 1; i < n - 1; i++) {
    l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
    mu[i] = h[i] / l[i];
    z[i] = (al[i] - h[i - 1] * z[i - 1]) / l[i];
  }

  l[n - 1] = 1;
  z[n - 1] = 0;

  // backwards substitution
  std::vector<T> b(n, 0), c(n, 0), d(n, 0);
  c[n - 1] = 0;

  std::vector<std::tuple<T, T, T, T>> coeff(n - 1);
  for (std::size_t j = n - 1; j-- > 0;) {
    c[j] = z[j] - mu[j] * c[j + 1];
    b[j] = (fx[j + 1] - fx[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
    d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
    coeff[j] = {fx[j], b[j], c[j], d[j]};
  }
  return coeff;
}

/*
 *
 * Cubic conditioned spline
 *
 */
template <std::floating_point T>
std::vector<std::tuple<T, T, T, T>> conditioned(const std::vector<T> &x, const std::vector<T> &fx,
                           T FPO, T FPN) {
  std::size_t n = x.size();
  if (n != fx.size()) {
    throw std::invalid_argument("x and fx have different sizes");
  }

  std::vector<T> h(n - 1);
  for (std::size_t i = 0; i < n - 1; i++) {
    h[i] = x[i + 1] - x[i];
  }

  std::vector<T> al(n);
  al[0] = 3 * (fx[1] - fx[0]) / h[0] - 3 * FPO;
  al[n - 1] = 3 * FPN - 3 * (fx[n - 1] - fx[n - 2]) / h[n - 2];
  for (std::size_t i = 1; i < n - 1; i++) {
    al[i] =
        3.0 * (fx[i + 1] - fx[i]) / h[i] - 3.0 * (fx[i] - fx[i - 1]) / h[i - 1];
  }

  // solve tridiagonal system
  std::vector<T> l(n), mu(n), z(n);

  l[0] = 2 * h[0];
  mu[0] = 0.5;
  z[0] = al[0] / l[0];

  for (std::size_t i = 1; i < n - 1; i++) {
    l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
    mu[i] = h[i] / l[i];
    z[i] = (al[i] - h[i - 1] * z[i - 1]) / l[i];
  }

  l[n - 1] = h[n - 2] * (2 - mu[n - 2]);
  z[n - 1] = (al[n - 1] - h[n - 2] * z[n - 2]) / l[n - 1];

  // backwards substitution
  std::vector<T> b(n), c(n), d(n);
  c[n - 1] = z[n - 1];

  std::vector<std::tuple<T, T, T, T>> coeff(n - 1);
  for (std::size_t j = n - 1; j-- > 0;) {
    c[j] = z[j] - mu[j] * c[j + 1];
    b[j] = (fx[j + 1] - fx[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
    d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
    coeff[j] = {fx[j], b[j], c[j], d[j]};
  }
  return coeff;
}
} // namespace spline
} // namespace interp
