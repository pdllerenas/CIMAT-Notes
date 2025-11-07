#include <concepts>
#include <stdexcept>
#include <vector>

template <std::floating_point T> class matrix {
  std::size_t m_rows{}, m_cols{};
  std::vector<T> m_data;

public:
  matrix() = default;
  matrix(std::size_t rows, std::size_t cols, T init_val = T{})
      : m_rows(rows), m_cols(cols), m_data(rows * cols, init_val) {}

  class matrix_row {
    const T *row_start;
    std::size_t row_length;

  public:
    matrix_row(T *start, std::size_t len) : row_start(start), row_length(len) {}
    T &operator[](std::size_t j) {
      if (j >= row_length) {
        throw std::out_of_range("columns index out of range");
        ;
      }
      return row_start[j];
    }
  };

	// usual [][] accessor
  matrix_row operator[](std::size_t i) {
    if (i >= m_rows) {
      throw std::out_of_range("row index out of range");
    }
    return matrix_row(&m_data[i * m_cols], m_cols);
  }

	// direct access
  T &operator()(std::size_t i, std::size_t j) {
    if (i >= m_rows || j >= m_cols) {
      throw std::out_of_range("index out of range");
    }
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
};
