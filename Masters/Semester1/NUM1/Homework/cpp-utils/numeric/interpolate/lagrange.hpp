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
