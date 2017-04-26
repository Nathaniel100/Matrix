//
// Created by 吴凡 on 2017/4/23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include "matrix_traits.h"

#include <cstddef>
#include <initializer_list>
#include <vector>
#include <array>
namespace matrix {

template<size_t N>
struct MatrixSlice {
  MatrixSlice() = default;

  MatrixSlice(size_t s, std::initializer_list<size_t> exts);
  MatrixSlice(size_t s, std::initializer_list<size_t> exts, std::initializer_list<size_t> strs);

  template<typename... Dims>
  MatrixSlice(Dims... dims);

  template<typename... Dims, typename Enable = EnableIf<All(Convertible<Dims, size_t>()...)>>
  size_t operator()(Dims... dims) const;

  size_t size;
  size_t start;
  std::array<size_t, N> extents;
  std::array<size_t, N> strides;
};

template<typename T, size_t N>
class MatrixRef;

template<typename T, size_t N>
class Matrix {
public:
  static constexpr size_t order = N;
  using value_type = T;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  Matrix() = default;
  Matrix &Matrix(const Matrix<T, N> &m) = default;
  Matrix &operator=(const Matrix<T, N> &m) = default;
  Matrix &Matrix(Matrix<T, N> &&m) = default;
  Matrix &operator=(Matrix<T, N> &&m) = default;
  ~Matrix() = default;

  template<typename E>
  Matrix(const MatrixRef<E, N> &other);
  template<typename E>
  Matrix &operator=(const MatrixRef<E, N> &other);

  Matrix(MatrixInitializer<T, N> initializer);
  Matrix &operator=(MatrixInitializer<T, N> initializer);

  template<typename... Exts>
  Matrix(Exts... exts);

  template<typename E>
  Matrix(std::initializer_list<E>) = delete;
  template<typename E>
  Matrix &operator=(std::initializer_list<E>) = delete;

  size_t extent(size_t i) const {
    return desc.extents[i];
  }

  size_t size() const {
    return elems.size();
  }

  const MatrixSlice<N> &descriptor() const {
    return desc;
  }

  T *data() {
    return elems.data();
  }

  const T *data() const {
    return elems.data();
  }

  iterator begin() {
    return elems.begin();
  }

  const_iterator begin() const {
    return elems.begin();
  }

  const_iterator cbegin() const {
    return elems.cbegin();
  }

  iterator end() {
    return elems.end();
  }

  const_iterator end() const {
    return elems.end();
  }

  const_iterator cend() const {
    return elems.cend();
  }


  MatrixRef<T, N - 1> operator[](size_t i) {
    return row(i);
  };
  MatrixRef<const T, N - 1> operator[](size_t i) const {
    return row(i);
  };

  MatrixRef<T, N - 1> row(size_t i);
  MatrixRef<const T, N - 1> row(size_t i) const;

  MatrixRef<T, N - 1> col(size_t i);
  MatrixRef<const T, N - 1> col(size_t i) const;

  template<typename... Args>
  EnableIf<RequiringElement<Args...>(), T &> operator()(Args... args);
  template<typename... Args>
  EnableIf<RequiringElement<Args...>(), const T &> operator()(Args... args) const;
  template<typename... Args>
  EnableIf<RequiringSlice<Args...>(), MatrixRef<T, N> > operator()(const Args &... args);
  template<typename... Args>
  EnableIf<RequiringSlice<Args...>(), MatrixRef<const T, N> > operator()(const Args &... args) const;

  template<typename F> Matrix &apply(F f);
  template<typename M, typename F> Matrix &apply(const M &m, F f);

  Matrix &operator=(const T &value);
  Matrix &operator+=(const T &value);
  Matrix &operator-=(const T &value);
  Matrix &operator*=(const T &value);
  Matrix &operator/=(const T &value);
  Matrix &operator%=(const T &value);

  template<typename M> Matrix &operator+=(const M &);
  template<typename M> Matrix &operator-=(const M &);

private:
  MatrixSlice<N> desc;
  std::vector<T> elems;
};

template<typename T, size_t N>
Matrix<T, N> operator+(const Matrix<T, N> &a, const Matrix<T, N> &b);

template<typename T1, typename T2, size_t N, typename RT = Matrix<CommonType<ValueType<T1>, ValueType<T2>>, N> >
Matrix<RT, N> operator+(const Matrix<T1, N> &a, const Matrix<T2, N> &b);

}

#endif //MATRIX_MATRIX_H
