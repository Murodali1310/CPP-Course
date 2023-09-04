#pragma once

#include <algorithm>
#include <iostream>

template <size_t H, size_t W, typename T = int64_t>
class Matrix {
 private:
  T a_[H][W];

 public:
  typedef Matrix<std::max<size_t>(H - 1, 1), std::max<size_t>(W - 1, 1), T>
      MatrixMinor;
  Matrix() {
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < W; ++j) {
        a_[i][j] = 0;
      }
    }
  }
  explicit Matrix(const T& elem) {
    for (size_t idx = 0; idx < H; ++idx) {
      for (size_t jdx = 0; jdx < W; ++jdx) {
        a_[idx][jdx] = elem;
      }
    }
  }
  T& operator()(const size_t& idx, const size_t& jdx) { return a_[idx][jdx]; }
  const T& operator()(const size_t& idx, const size_t& jdx) const {
    return a_[idx][jdx];
  }
  Matrix(const std::vector<std::vector<T>>& matrix) {
    for (size_t idx = 0; idx < H; ++idx) {
      for (size_t jdx = 0; jdx < W; ++jdx) {
        a_[idx][jdx] = matrix[idx][jdx];
      }
    }
  }
  bool operator==(const Matrix& rhs) const {
    for (size_t idx = 0; idx < H; ++idx) {
      for (size_t jdx = 0; jdx < W; ++jdx) {
        if (a_[idx][jdx] != rhs(idx, jdx)) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrix& rhs) const { return *this != rhs; }
  template <size_t W1>
  friend Matrix<H, W1, T> operator*(const Matrix<H, W, T>& lhs,
                                    const Matrix<W, W1, T>& rhs) {
    Matrix<H, W1, T> matr;
    for (size_t i = 0; i < H; ++i) {
      for (size_t k = 0; k < W; ++k) {
        for (size_t j = 0; j < W1; ++j) {
          matr(i, j) += lhs(i, k) * rhs(k, j);
        }
      }
    }
    return matr;
  }
  friend Matrix operator*(const T& lhs, const Matrix& rhs) {
    Matrix matr;
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < W; ++j) {
        matr(i, j) = lhs * rhs(i, j);
      }
    }
    return matr;
  }
  friend Matrix operator*(const Matrix& lhs, const T& rhs) { return rhs * lhs; }
  template <size_t W1>
  Matrix& operator*=(const Matrix<W, W1, T>& rhs) {
    return *this = *this * rhs;
  }
  Matrix& operator*=(const T& rhs) { return *this = *this * rhs; }
  Matrix operator+() const { return *this; }
  Matrix operator-() const { return *this * -1; }
  Matrix& operator+=(const Matrix& rhs) {
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < W; ++j) {
        a_[i][j] += rhs(i, j);
      }
    }
    return *this;
  }
  friend Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    return Matrix(lhs) += rhs;
  }
  Matrix& operator-=(const Matrix& rhs) { return *this += -rhs; }
  friend Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    return Matrix(lhs) -= rhs;
  }
  Matrix<W, H, T> Transposed() const {
    Matrix<W, H, T> matr;
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < W; ++j) {
        matr(j, i) = a_[i][j];
      }
    }
    return matr;
  }
  T Trace() const {
    T matr = 0;
    for (size_t i = 0; i < std::min(W, H); ++i) {
      matr += a_[i][i];
    }
    return matr;
  }
};

template <size_t H, typename T>
class Matrix<H, H, T> {
 private:
  T a_[H][H];

 public:
  typedef Matrix<std::max<size_t>(H - 1, 1), std::max<size_t>(H - 1, 1), T>
      MatrixMinor;
  Matrix() {
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < H; ++j) {
        a_[i][j] = 0;
      }
    }
  }
  explicit Matrix(const T& elem) {
    for (size_t idx = 0; idx < H; ++idx) {
      for (size_t jdx = 0; jdx < H; ++jdx) {
        a_[idx][jdx] = elem;
      }
    }
  }
  T& operator()(const size_t& idx, const size_t& jdx) { return a_[idx][jdx]; }
  const T& operator()(const size_t& idx, const size_t& jdx) const {
    return a_[idx][jdx];
  }
  Matrix(const std::vector<std::vector<T>>& matrix) {
    for (size_t idx = 0; idx < H; ++idx) {
      for (size_t jdx = 0; jdx < H; ++jdx) {
        a_[idx][jdx] = matrix[idx][jdx];
      }
    }
  }
  bool operator==(const Matrix& rhs) const {
    for (size_t idx = 0; idx < H; ++idx) {
      for (size_t jdx = 0; jdx < H; ++jdx) {
        if (a_[idx][jdx] != rhs(idx, jdx)) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrix& rhs) const { return *this != rhs; }
  template <size_t W1>
  friend Matrix<H, W1, T> operator*(const Matrix<H, H, T>& lhs,
                                    const Matrix<H, W1, T>& rhs) {
    Matrix<H, W1, T> matr;
    for (size_t i = 0; i < H; ++i) {
      for (size_t k = 0; k < H; ++k) {
        for (size_t j = 0; j < W1; ++j) {
          matr(i, j) += lhs(i, k) * rhs(k, j);
        }
      }
    }
    return matr;
  }
  friend Matrix operator*(const T& lhs, const Matrix& rhs) {
    Matrix matr;
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < H; ++j) {
        matr(i, j) = lhs * rhs(i, j);
      }
    }
    return matr;
  }
  friend Matrix operator*(const Matrix& lhs, const T& rhs) { return rhs * lhs; }
  template <size_t W1>
  Matrix& operator*=(const Matrix<H, W1, T>& rhs) {
    return *this = *this * rhs;
  }
  Matrix& operator*=(const T& rhs) { return *this = *this * rhs; }
  Matrix operator+() const { return *this; }
  Matrix operator-() const { return *this * -1; }
  Matrix& operator+=(const Matrix& rhs) {
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < H; ++j) {
        a_[i][j] += rhs(i, j);
      }
    }
    return *this;
  }
  friend Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    return Matrix(lhs) += rhs;
  }
  Matrix& operator-=(const Matrix& rhs) { return *this += -rhs; }
  friend Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    return Matrix(lhs) -= rhs;
  }
  Matrix<H, H, T> Transposed() const {
    Matrix<H, H, T> matr;
    for (size_t i = 0; i < H; ++i) {
      for (size_t j = 0; j < H; ++j) {
        matr(j, i) = a_[i][j];
      }
    }
    return matr;
  }
  T Trace() const {
    T matr = 0;
    for (size_t i = 0; i < H; ++i) {
      matr += a_[i][i];
    }
    return matr;
  }
};