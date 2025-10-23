#include <concepts>

namespace interp {
// allows float, double and long double
template <typename F, std::floating_point T>
T lagrange(T *x, T *y, T z, std::size_t n) {
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
} // namespace interp
