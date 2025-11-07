#include <concepts>
#include <stdexcept>
#include <vector>

namespace factorization {
namespace crout {
// allows float, double and long double
template <std::floating_point T>
std::vector<T> tridiagonal(const std::vector<T> &a, const std::vector<T> &b,
                           const std::vector<T> &c, const std::vector<T> &d) {
  std::size_t n = b.size();
  if (a.size() != n - 1 || c.size() != n - 1 || d.size() != n) {
  }
  // Since the original matrix is tridiagonal,
  // the lower matrix will be only 2 diagonals

  // lower matrix, lower diagonal
  std::vector<T> cp(n - 1);
  std::vector<T> dp(n);

  T denom = b[0];
  if (denom == T(0)) {
    throw std::runtime_error("zero pivot");
  }
  cp[0] = c[0] / denom;
  dp[0] = d[0] / denom;

  for (std::size_t i = 1; i < n - 1; i++) {
    denom = b[i] - a[i - 1] * cp[i - 1];
    if (denom == T(0)) {
      throw std::runtime_error("zero pivot");
    }
    cp[i] = c[i] / denom;
    dp[i] = (d[i] - a[i - 1] * dp[i - 1]) / denom;
  }

  if (n > 1) {
    denom = b[n - 1] - a[n - 1] * cp[n - 2];
    if (denom == T(0)) {
      throw std::runtime_error("zero pivot");
    }
    dp[n - 1] = (d[n - 1] - a[n - 2] * dp[n - 2]) / denom;
  }

  std::vector<T> x(n);
  x[n - 1] = dp[n - 1];
  for (int i = static_cast<int>(n) - 2; i >= 0; i--) {
    x[i] = dp[i] - x[i + 1];
  }
	return x;
}

template <std::floating_point T>
std::vector<T> clamped(const std::vector<T> &x, const std::vector<T> &fx) {
  std::size_t n = x.size();
  if (n != fx.size()) {
    throw std::invalid_argument("x and fx have different sizes");
  }
}
} // namespace crout
} // namespace factorization
