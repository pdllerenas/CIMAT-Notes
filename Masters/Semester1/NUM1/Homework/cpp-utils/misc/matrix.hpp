#pragma once
#include <execution>
#define PAR std::execution::par,

#include <algorithm>
#include <cmath>
#include <concepts>
#include <iostream>
#include <stdexcept>
#include <vector>

template <std::floating_point T> class matrix {
  std::size_t m_rows{}, m_cols{};
  std::vector<T> m_data;

public:
  matrix() = default;
  matrix(std::size_t rows, std::size_t cols, T init_val = T{})
      : m_rows(rows), m_cols(cols), m_data(rows * cols, init_val) {}
  auto begin() { return m_data.begin(); }
  auto end() { return m_data.end(); }
  auto begin() const { return m_data.begin(); }
  auto end() const { return m_data.end(); }

  // ===========================
  // Accessors
  // ===========================
  class matrix_row {
    T *row_start;
    std::size_t row_length;

  public:
    matrix_row(T *start, std::size_t len) : row_start(start), row_length(len) {}
    T &operator[](std::size_t j) {
      if (j >= row_length)
        throw std::out_of_range("column index out of range");
      return row_start[j];
    }
  };

  void swap(size_t r1, size_t r2) {
    const size_t ncols = (*this).cols();
    for (size_t j = 0; j < ncols; ++j) {
      std::swap((*this)(r1, j), (*this)(r2, j));
    }
  }

  matrix_row operator[](std::size_t i) {
    if (i >= m_rows)
      throw std::out_of_range("row index out of range");
    return matrix_row(&m_data[i * m_cols], m_cols);
  }

  T &operator()(std::size_t i, std::size_t j) {
    if (i >= m_rows || j >= m_cols)
      throw std::out_of_range("index out of range");
    return m_data[i * m_cols + j];
  }

  const T &operator()(std::size_t i, std::size_t j) const {
    if (i >= m_rows || j >= m_cols)
      throw std::out_of_range("index out of range");
    return m_data[i * m_cols + j];
  }

  T *data() { return m_data.data(); }
  const T *data() const { return m_data.data(); }
  std::size_t rows() const noexcept { return m_rows; }
  std::size_t cols() const noexcept { return m_cols; }
  std::size_t size() const noexcept { return m_data.size(); }

  void fill(T value) { std::fill(m_data.begin(), m_data.end(), value); }
  void resize(std::size_t new_rows, std::size_t new_cols, T init_val = T{}) {
    m_rows = new_rows;
    m_cols = new_cols;
    m_data.assign(new_rows * new_cols, init_val);
  }

  // ===========================
  // Linear Algebra Methods
  // ===========================

  matrix<T> transpose() const {
    matrix<T> AT(m_cols, m_rows);
    for (size_t i = 0; i < m_rows; ++i)
      for (size_t j = 0; j < m_cols; ++j)
        AT(j, i) = (*this)(i, j);
    return AT;
  }

  matrix<T> operator+(const matrix<T> &other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols)
      throw std::invalid_argument("matrix dimension mismatch in operator+");

    matrix<T> result(m_rows, m_cols, T{});
    for (size_t i = 0; i < m_rows; i++)
      for (size_t j = 0; j < m_cols; j++)
        result(i, j) = (*this)(i, j) + other(i, j);
    return result;
  }

  matrix<T> operator-(const matrix<T> &other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols)
      throw std::invalid_argument("matrix dimension mismatch in operator-");

    matrix<T> result(m_rows, m_cols, T{});
    for (size_t i = 0; i < m_rows; i++)
      for (size_t j = 0; j < m_cols; j++)
        result(i, j) = (*this)(i, j) - other(i, j);
    return result;
  }
  matrix<T> operator*(const matrix<T> &other) const {
    if (m_cols != other.m_rows)
      throw std::invalid_argument("matrix dimension mismatch in operator*");
    matrix<T> C(m_rows, other.m_cols, T{});
    for (size_t i = 0; i < m_rows; ++i)
      for (size_t j = 0; j < other.m_cols; ++j)
        for (size_t k = 0; k < m_cols; ++k)
          C(i, j) += (*this)(i, k) * other(k, j);
    return C;
  }

  std::vector<T> operator*(const std::vector<T> &v) const {
    if (m_cols != v.size()) {
      throw std::invalid_argument("matrix dimension mismatch in operator*");
    }

    std::vector<T> result(m_rows, T{});

    std::for_each(std::execution::par, result.begin(), result.end(),
                  [&](T &res) {
                    size_t i = &res - result.data(); // compute row index
                    T sum = 0;
                    for (size_t j = 0; j < m_cols; ++j)
                      sum += m_data[i * m_cols + j] * v[j];
                    res = sum;
                  });

    return result;
  }

  // returns inverse of matrix using gaussian elimination with pivoting
  matrix<T> inverse() const {
    if (m_rows != m_cols)
      throw std::invalid_argument("matrix must be square to invert");

    std::size_t n = m_rows;
    matrix<T> A(*this);
    matrix<T> I(n, n);
    for (size_t i = 0; i < n; ++i)
      I(i, i) = 1;

    for (size_t i = 0; i < n; ++i) {
      T pivot = A(i, i);
      if (std::abs(pivot) < 1e-12)
        throw std::runtime_error("matrix is singular or nearly singular");

      for (size_t j = 0; j < n; ++j) {
        A(i, j) /= pivot;
        I(i, j) /= pivot;
      }
      for (size_t k = 0; k < n; ++k) {
        if (k == i)
          continue;
        T factor = A(k, i);
        for (size_t j = 0; j < n; ++j) {
          A(k, j) -= factor * A(i, j);
          I(k, j) -= factor * I(i, j);
        }
      }
    }
    return I;
  }

  // ===========================
  // Utilities
  // ===========================
  void print() const {
    for (size_t i = 0; i < m_rows; ++i) {
      for (size_t j = 0; j < m_cols; ++j)
        std::cout << (*this)(i, j) << " ";
      std::cout << "\n";
    }
  }
};

// scalar product: A * lambda
template <std::floating_point T>
matrix<T> operator*(const matrix<T> &A, T lambda) {
  matrix<T> R = A;

  std::for_each(std::execution::par, R.data(), R.data() + R.size(),
                [&](T &x) { x *= lambda; });

  return R;
}
// scalar product: lambda * A
template <std::floating_point T>
matrix<T> operator*(T lambda, const matrix<T> &A) {
  return A * lambda; // reuse above implementation
}

// vector * matrix
template <std::floating_point T>
std::vector<T> operator*(const std::vector<T> &v, const matrix<T> &M) {
  if (v.size() != M.rows())
    throw std::invalid_argument("dimension mismatch");

  std::vector<T> result(M.cols(), T{});

  std::for_each(std::execution::par, result.begin(), result.end(), [&](T &res) {
    size_t j = &res - result.data();
    T s{};
    for (size_t i = 0; i < M.rows(); ++i)
      s += v[i] * M(i, j);
    res = s;
  });

  return result;
}
// outer product of a and b: a b^T
template <std::floating_point T>
matrix<T> outer_product(const std::vector<T> &a, const std::vector<T> &b) {
  if (a.size() != b.size())
    throw std::invalid_argument("outer_product: vector size mismatch");

  size_t n = a.size();
  matrix<T> M(n, n);

  std::for_each(std::execution::par, M.data(), M.data() + M.size(),
                [&](T &cell) {
                  size_t idx = &cell - M.data();
                  size_t i = idx / n;
                  size_t j = idx % n;
                  cell = a[i] * b[j];
                });

  return M;
}

// l2 norm of v
template <std::floating_point T> T norm(const std::vector<T> &v) {
  T sum = std::transform_reduce(std::execution::par, v.begin(), v.end(), T{0},
                                std::plus<T>(), [](T x) { return x * x; });

  return std::sqrt(sum);
}

// parallel addition of vectors
template <std::floating_point T>
std::vector<T> operator+(const std::vector<T> &v, const std::vector<T> &w) {
  if (v.size() != w.size())
    throw std::invalid_argument("vector dimension mismatch in operator+");

  std::vector<T> u(v);

  std::transform(std::execution::par, u.begin(), u.end(), w.begin(), u.begin(),
                 std::plus<>{});

  return u;
}

// parallel subtraction of vectors
template <std::floating_point T>
std::vector<T> operator-(const std::vector<T> &v, const std::vector<T> &w) {
  if (v.size() != w.size())
    throw std::invalid_argument("vector dimension mismatch in operator-");

  std::vector<T> u(v);

  std::transform(std::execution::par, u.begin(), u.end(), w.begin(), u.begin(),
                 std::minus<>{});

  return u;
}

template <std::floating_point T>
std::vector<T> operator*(const std::vector<T> &v, T lambda) {
  std::vector<T> u(v);
  std::transform(std::execution::par, u.begin(), u.end(), u.begin(),
                 [lambda](T x) { return x * lambda; });
  return u;
}

template <std::floating_point T>
std::vector<T> operator*(T lambda, const std::vector<T> &v) {
  return v * lambda;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const matrix<T> &M) {
  for (size_t i = 0; i < M.rows(); ++i) {
    for (size_t j = 0; j < M.cols(); ++j) {
      os << M(i, j);
      if (j + 1 < M.cols())
        os << ",";
    }
    os << "\n";
  }
  return os;
}

template <typename T> matrix<T> operator/(const matrix<T> &A, T scalar) {
  matrix<T> result(A.rows(), A.cols());
  for (size_t i = 0; i < A.rows(); ++i)
    for (size_t j = 0; j < A.cols(); ++j)
      result(i, j) = A(i, j) / scalar;
  return result;
}
