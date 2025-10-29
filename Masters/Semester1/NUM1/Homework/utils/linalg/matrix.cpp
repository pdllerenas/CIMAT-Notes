#include "matrix.hpp"
#include <vector>

using std::vector;

template <class T> class Matrix {
  vector<T> data;
  size_t cols;

public:
  template <class U> class MatrixIndexer {
    size_t offset;
    vector<U> &data;

  public:
    MatrixIndexer(size_t offset, vector<U> &data)
        : offset(offset), data(data) {}
    T &operator[](size_t index) { return data[offset + index]; }
  };
  Matrix(size_t r, size_t c) : data(r * c), cols(c) {}
  Matrix<T> operator[](size_t index) { return Matrix<T>(index * cols, data); }
};
