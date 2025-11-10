#pragma once
#include <concepts>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <iostream>

template <std::floating_point T>
class matrix {
  std::size_t m_rows{}, m_cols{};
  std::vector<T> m_data;

public:
  matrix() = default;
  matrix(std::size_t rows, std::size_t cols, T init_val = T{})
      : m_rows(rows), m_cols(cols), m_data(rows * cols, init_val) {}

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
  // Core Linear Algebra Methods
  // ===========================

  matrix<T> transpose() const {
    matrix<T> AT(m_cols, m_rows);
    for (size_t i = 0; i < m_rows; ++i)
      for (size_t j = 0; j < m_cols; ++j)
        AT(j, i) = (*this)(i, j);
    return AT;
  }

  matrix<T> operator*(const matrix<T> &B) const {
    if (m_cols != B.m_rows)
      throw std::invalid_argument("matrix dimension mismatch in operator*");
    matrix<T> C(m_rows, B.m_cols, T{});
    for (size_t i = 0; i < m_rows; ++i)
      for (size_t j = 0; j < B.m_cols; ++j)
        for (size_t k = 0; k < m_cols; ++k)
          C(i, j) += (*this)(i, k) * B(k, j);
    return C;
  }

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

