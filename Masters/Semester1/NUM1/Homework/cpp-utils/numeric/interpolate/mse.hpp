#include "../../misc/matrix.hpp"
#include "../solvers/Cholesky.hpp"
#include "../solvers/Crout.hpp"

#include <cmath>
#include <concepts>
#include <numbers>
#include <stdexcept>
#include <vector>

inline double phi_trig(size_t k, double x) {
  return cos((k * std::numbers::pi * x) / 6.0);
}

inline double phi_poly(size_t k, double x) {
  double res = 1.0;
  for (size_t i = 0; i < k; i++) {
    res *= x;
  }
  return res;
}

inline double phi_rad(double x, double xi) {
  return std::exp(-(x - xi) * (x - xi));
}

inline double phi_logistic(size_t k, double t) {
  if (k == 0)
    return 1.0;
  if (k == 1)
    return std::exp(-3.0 * t);
  return 0.0;
}

template <std::floating_point T> static inline T phi_exp(size_t k, T x) {
  return cos((k * std::numbers::pi * x) / 6.0);
}

template <std::floating_point T>
static inline void add_regularization(matrix<T> &G, double lambda) {
  size_t m = G.rows();
  for (size_t i = 0; i < m; ++i)
    G[i][i] += lambda;
}

template <std::floating_point T, typename F>
static inline matrix<T> generate_Phi(const std::vector<T> &xs, F phi,
                                     size_t n) {
  size_t p = xs.size();
  matrix<T> Phi(p, n);

  for (size_t i = 0; i < p; ++i) {
    for (size_t k = 0; k < n; ++k) {
      Phi(i, k) = phi(k, xs[i]);
    }
  }
  return Phi;
}

template <std::floating_point T, typename F>
static inline matrix<T> generate_RBF(const std::vector<T> &x,
                                     const std::vector<T> &centers) {
  size_t p = x.size();
  size_t m = centers.size();
  matrix<T> Phi(p, m);

  for (size_t i = 0; i < p; ++i) {
    for (size_t k = 0; k < m; ++k) {
      Phi(i, k) = std::exp(-std::pow(x[i] - centers[k], 2));
    }
  }
  return Phi;
}

template <std::floating_point T>
static void build_system(const matrix<T> &Phi, const std::vector<T> &y,
                         matrix<T> &G, std::vector<T> &rhs) {
  size_t p = Phi.rows();
  size_t m = Phi.cols();

  for (size_t r = 0; r < p; ++r) {
    for (size_t i = 0; i < m; ++i) {
      T pir = Phi(r, i);
      rhs[i] += pir * y[r];
      for (size_t j = i; j < m; ++j) {
        G(i, j) += pir * Phi(r, j);
      }
    }
  }

  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < i; ++j) {
      G(i, j) = G(j, i);
    }
  }
}

namespace interp {
// allows float, double and long double
namespace mse {
/*
 * @brief Given m observation points (x,y), find the interpolating polynomial of
 * degree at most m - 2
 *
 * @param x points given
 * @param y points given
 *
 * @return coefficients of the interpolating polynomial
 */
template <std::floating_point T>
std::vector<T> polynomial(const std::vector<T> &x, const std::vector<T> &y,
                          T lambda, unsigned short degree) {
  size_t m = x.size();
  if (m != y.size()) {
    throw std::invalid_argument("x and y must have the same size");
  }
  if (degree >= m - 1) {
    throw std::invalid_argument("degree must be lower than vector size");
  }

  matrix<T> A(degree + 1, degree + 1, 0);
  for (size_t i = 0; i <= degree; ++i) {
    for (size_t j = i; j <= degree; ++j) {
      for (size_t k = 0; k < m; ++k) {
        A(i, j) += std::pow(x[k], i + j);
      }
      A(j, i) = A(i, j);
    }
  }

  std::vector<T> b(degree + 1, 0);
  for (size_t i = 0; i <= degree; ++i) {
    for (size_t k = 0; k < m; ++k) {
      b[i] += y[k] * std::pow(x[k], i);
    }
  }
  if (lambda > 0.0) {
    add_regularization(A, lambda);
  }

  return solvers::cholesky::solve(A, b);
}

template <std::floating_point T>
std::vector<T> cosine(const std::vector<T> &x, const std::vector<T> &y,
                      T lambda, size_t n) {
  matrix<T> Phi = generate_Phi(x, phi_trig, n + 1);
  matrix<T> G(n + 1, n + 1, 0);
  std::vector<T> rhs(n + 1, 0);

  build_system(Phi, y, G, rhs);

  if (lambda > 0.0)
    add_regularization(G, lambda);

  std::vector<size_t> pivots;
  solvers::crout::factorize(G, pivots);

  return solvers::crout::solve(G, pivots, rhs);
}

template <std::floating_point T>
std::vector<T> radial(const std::vector<T> &x, const std::vector<T> &y,
                      T lambda) {
  size_t n = x.size();
  matrix<T> Phi(n, n, 0);

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      T diff = x[i] - x[j];
      Phi(i, j) = std::exp(-diff * diff);
    }
  }

  if (lambda > 0.0)
    add_regularization(Phi, lambda);

  std::vector<size_t> pivots;
  solvers::crout::factorize(Phi, pivots);

  return solvers::crout::solve(Phi, pivots, y);
}

template <std::floating_point T> T eval(T x, const std::vector<T> &coeff) {
  T result = 0;
  T term = 1;
  for (size_t i = 0; i < coeff.size(); i++) {
    result += coeff[i] * term;
    term *= x;
  }
  return result;
}

template <std::floating_point T, typename F>
T eval_basis(T x, const std::vector<T> &coeff, F phi) {
  T result = 0;
  for (size_t k = 0; k < coeff.size(); ++k)
    result += coeff[k] * phi(k, x);
  return result;
}

template <std::floating_point T>
T eval_basis_rad(T x, const std::vector<T> &coeff, const std::vector<T> &xs) {
  T result = 0;
  size_t k = 0;
  for (T xi : xs) {
    result += coeff[k++] * std::exp(-(x - xi) * (x - xi));
  }
  return result;
}

template <std::floating_point T>
std::vector<T> logistic_linearized(const std::vector<T> &x,
                                   const std::vector<T> &y, T lambda = 0.0) {
  size_t m = x.size();
  if (m != y.size()) {
    throw std::invalid_argument("x and y must have the same size");
  }

  std::vector<T> y_inv(m);
  for (size_t i = 0; i < m; ++i) {
    y_inv[i] = 1.0 / y[i];
  }

  size_t num_params = 2;
  matrix<T> Phi = generate_Phi(x, phi_logistic, num_params);

  matrix<T> G(num_params, num_params, 0);
  std::vector<T> rhs(num_params, 0);

  build_system(Phi, y_inv, G, rhs);

  if (lambda > 0.0) {
    add_regularization(G, lambda);
  }

  return solvers::cholesky::solve(G, rhs);
}

} // namespace mse

} // namespace interp
