//
// Created by 吴凡 on 2017/4/26.
//


#ifndef MATRIX_MATRIX_TRAITS_H
#define MATRIX_MATRIX_TRAITS_H

#include<type_traits>

namespace matrix {

namespace details {

template<typename T, size_t N>
class MatrixInit {
public:
  using type = std::initializer_list<typename MatrixInit<T, N - 1>::type>;
};

template<typename T>
class MatrixInit<T, 1> {
public:
  using type = std::initializer_list<T>;
};

template<typename T>
class MatrixInit<T, 0>;

} // end namespace details

template<typename T, size_t N>
using MatrixInitializer = typename details::MatrixInit<T, N>::type;


template<bool Pred, typename T = void>
using EnableIf = typename std::enable_if<Pred, T>::type;

template<typename From, typename To>
using Convertible = std::is_convertible<From, To>;

constexpr bool All() {
  return true;
}

template<typename... Args>
constexpr bool All(bool b, Args... args) {
  return b && All(args);
}

constexpr bool Some() {
  return false;
}

template<typename... Args>
constexpr bool Some(bool b, Args... args) {
  return b || Some(Args);
}

template<typename... Args>
constexpr bool RequiringElement() {
  return All(Convertible<Args, size_t>()...);
}

template<typename... Args>
constexpr bool RequiringSlice() {
  return All((Convertible<Args, size_t>() || Same<Args, Slice>())...) && Some(Same<Args, Slice>()...);
}

template<typename M>
using ValueType = typename M::value_type;

template<typename A, typename B>
using CommonType = typename std::common_type<A, B>::type;

}
#endif //MATRIX_MATRIX_TRAITS_H
