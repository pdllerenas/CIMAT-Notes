#include <concepts>
#include <stdexcept>
#include <vector>

namespace interp {
// allows float, double and long double
template <std::floating_point T> T lagrange(T *x, T *y, T z, std::size_t n) {
  T l;
  T val = 0;
  for (int i = 0; i < n; i++) {
    l = y[i];
    for (int j = 0; j < n; j++) {
      if (i != j) {
        l *= (z - x[j]) / (x[i] - x[j]);
      }
    }
    val += l;
  }
  return val;
}

template <std::floating_point T>
std::vector<T> lagrange_coeffs(const std::vector<T> &x,
                               const std::vector<T> &y) {
  if (x.size() != y.size()) {
    throw std::invalid_argument("x and y must have the same size");
  }

  size_t n = x.size();
  std::vector<T> total_coeffs(n, 0.0);

  for (size_t i = 0; i < n; ++i) {
    T weight = y[i];
    T denom = 1.0;
    for (size_t j = 0; j < n; ++j) {
      if (i != j) {
        denom *= (x[i] - x[j]);
      }
    }
    weight /= denom;

    std::vector<T> basis_coeffs = {1.0};

    for (size_t j = 0; j < n; ++j) {
      if (i != j) {
        std::vector<T> next_coeffs(basis_coeffs.size() + 1, 0.0);

        for (size_t k = 0; k < basis_coeffs.size(); ++k) {
          next_coeffs[k + 1] += basis_coeffs[k];
          next_coeffs[k] -= x[j] * basis_coeffs[k];
        }
        basis_coeffs = next_coeffs;
      }
    }

    for (size_t k = 0; k < n; ++k) {
      total_coeffs[k] += basis_coeffs[k] * weight;
    }
  }

  return total_coeffs;
}

template <std::floating_point T>
T lagrange(const std::vector<T> &x, const std::vector<T> &y, T z) {
  if (x.size() != y.size()) {
    throw std::invalid_argument("x and y must have the same size");
  }

  T l;
  T val = 0;
  for (std::size_t i = 0; i < x.size(); i++) {
    l = y[i];
    for (std::size_t j = 0; j < y.size(); j++) {
      if (i != j) {
        l *= (z - x[j]) / (x[i] - x[j]);
      }
    }
    val += l;
  }
  return val;
}
} // namespace interp
