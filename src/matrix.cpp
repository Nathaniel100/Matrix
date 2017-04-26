//
// Created by 吴凡 on 2017/4/25.
//

#include <matrix.h>
#include <assert.h>

namespace matrix {

namespace details {

template<size_t N>
template<typename... Dims>
size_t MatrixSlice<N>::operator()(Dims... dims) const {
  static_assert(sizeof...(Dims) == N, "");
  size_t args[N]{size_t(dims)...};
  return inner_product(args, args + N, strides.begin(), size_t(0));
}

}

template<typename T, size_t N>
template<typename... Exts>
Matrix<T, N>::Matrix(Exts... exts): desc{exts...}, elems(desc.size) {
}

template<typename T, size_t N>
Matrix<T, N>::Matrix(MatrixInitializer<T, N> initializer) {
};

template<typename T, size_t N>
Matrix &Matrix<T, N>::operator=(MatrixInitializer<T, N> initializer) {
  details::derive_extents(initializer, desc.extents);
  elems.reserve(desc.size);
  details::insert_flat(initializer, elems);
  assert(desc.size == elems.size());
}

template<typename T, size_t N>
template<typename E>
Matrix<T, N>::Matrix(const MatrixRef<E, N> &other): desc{other.desc}, elems{other.begin(), other.end()} {
  static_assert(Convertible<E, T>(), "Matrix constructor: incompatible element types");
}
template<typename T, size_t N>
template<typename E>
Matrix &Matrix<T, N>::operator=(const MatrixRef<E, N> &other) {
  static_assert(Convertible<E, T>(), "Matrix =: incompatible element types");
  desc = other.desc;
  elems.assign(other.begin(), other.end());
  return *this;
};

template<typename T, size_t N>
template<typename F>
Matrix &Matrix<T, N>::apply(F f) {
  for (auto &x : elems) { f(x); }
  return *this;
}

template<typename T, size_t N>
template<typename M, typename F>
Matrix &Matrix<T, N>::apply(const M &m, F f) {
  for(auto i = begin(), j = m.begin(); i != end(); ++i, ++j) {
    apply(*i, *j);
  }
  return *this;
};

template<typename T, size_t N>
Matrix &Matrix<T, N>::operator+=(const T &value) {
  return apply([&](T &v) { v += value; });
}

template<typename T, size_t N>
Matrix &Matrix<T, N>::operator-=(const T &value) {
  return apply([&](T &v) { v -= value; });
}

template<typename T, size_t N>
Matrix &Matrix<T, N>::operator*=(const T &value) {
  return apply([&](T &v) { v *= value; });
}

template<typename T, size_t N>
Matrix &Matrix<T, N>::operator/=(const T &value) {
  return apply([&](T &v) { v /= value; });
}

template<typename T, size_t N>
Matrix &Matrix<T, N>::operator%=(const T &value) {
  return apply([&](T &v) { v %= value; });
};


template<typename T, size_t N>
template<typename M>
Matrix &Matrix<T, N>::operator+=(const M &m) {
  return apply(m, [&](T& a, ValueType<M>& b) {
    a += b;
  });
};

template<typename T, size_t N>
template<typename M>
Matrix &Matrix<T, N>::operator-=(const M &m) {
  return apply(m, [&](T& a, ValueType<M>& b) {
    a -= b;
  });
};


template<typename T, size_t N>
Matrix<T, N> operator+(const Matrix<T, N> &a, const Matrix<T, N> &b) {
  Matrix<T, N> res = a;
  a += b;
  return res;
};

template<typename T1, typename T2, size_t N, typename RT = Matrix<CommonType<ValueType<T1>, ValueType<T2>>, N> >
Matrix<RT, N> operator+(const Matrix<T1, N> &a, const Matrix<T2, N> &b) {
  Matrix<RT, N> res = a;
  a += b;
  return res;
};


}
