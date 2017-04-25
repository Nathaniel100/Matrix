//
// Created by 吴凡 on 2017/4/23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include <cstddef>
#include <initializer_list>
#include <vector>
#include <array>
namespace matrix {

namespace details {

template<class T, size_t N>
class MatrixInit {
public:
  using type = std::initializer_list<typename MatrixInit<T, N - 1>::type>;
};

template<class T>
class MatrixInit<T, 1> {
public:
  using type = std::initializer_list<T>;
};

template<class T>
class MatrixInit<T, 0>;

}

template<size_t N>
struct MatrixSlice {
  MatrixSlice() = default;

  MatrixSlice(size_t s, std::initializer_list<size_t> exts);
  MatrixSlice(size_t s, std::initializer_list<size_t> exts, std::initializer_list<size_t> strs);

  template<typename... Dims>
  MatrixSlice(Dims... dims);

  template<typename... Dims, typename = Enable_if<All(Convertible<Dims, size_t>()...)>>
  size_t
  operator()(Dims... dims) const;

  size_t size;
  size_t start;
  std::array<size_t, N> extents;
  std::array<size_t, N> strides;
};

template<class T, size_t N>
class MatrixRef;


template<class T, size_t N>
using MatrixInitializer = typename details::MatrixInit<T, N>::type;

template<class T, size_t N>
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

  template<class E>
  Matrix(const MatrixRef<E, N> &other);
  template<class E>
  Matrix &operator=(const MatrixRef<E, N> &other);

  Matrix(MatrixInitializer<T, N> initializer);
  Matrix &operator=(MatrixInitializer<T, N> initializer);

  template<class... Exts>
  Matrix(Exts... exts);

  template<class E>
  Matrix(std::initializer_list<E>) = delete;
  template<class E>
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

private:
  MatrixSlice<N> desc;
  std::vector<T> elems;
};

}

#endif //MATRIX_MATRIX_H
