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

template<class T, size_t N>
template<class... Exts>
Matrix<T, N>::Matrix(Exts... exts): desc{exts...}, elems(desc.size) {
}

template<class T, size_t N>
Matrix<T, N>::Matrix(MatrixInitializer<T, N> initializer) {
};

template<class T, size_t N>
Matrix &Matrix<T, N>::operator=(MatrixInitializer<T, N> initializer) {
  details::derive_extents(initializer, desc.extents);
  elems.reserve(desc.size);
  details::insert_flat(initializer, elems);
  assert(desc.size == elems.size());
}

template<class T, size_t N>
template<class E>
Matrix<T, N>::Matrix(const MatrixRef<E, N> &other): desc{other.desc}, elems{other.begin(), other.end()} {
  static_assert(Convertible<E, T>(), "Matrix constructor: incompatible element types");
}
template<class T, size_t N>
template<class E>
Matrix &Matrix<T, N>::operator=(const MatrixRef<E, N> &other) {
  static_assert(Convertible<E, T>(), "Matrix =: incompatible element types");
  desc = other.desc;
  elems.assign(other.begin(), other.end());
  return *this;
};

}
