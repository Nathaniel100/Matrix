//
// Created by 吴凡 on 2017/4/23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H


namespace matrix {

namespace details {

template<size_t N>
class Matrix_Slice;

template<class T, size_t N>
class Matrix_Initializer;

template<class T, size_t N>
class Matrix_ref;


}


template<class T, size_t N>
class Matrix {
public:
    Matrix() = default;

    template<class... Exts>
    Matrix(Exts... exts);

    Matrix(details::Matrix_Initializer<T, N> initializer);

    template<class E>
    Matrix(std::initializer_list <E>) = delete;

    Matrix &Matrix(const Matrix<T, N> &m) = default;

    Matrix &operator=(const Matrix<T, N> &m) = default;

    Matrix &Matrix(Matrix<T, N> &&m) = default;

    Matrix &operator=(const Matrix<T, N> &m) = default;

    template<class E>
    Matrix(const Matrix_ref<E, N> &other);


private:
    details::Matrix_Slice<N> desc;
    std::vector <T> elems;
};


}

#endif //MATRIX_MATRIX_H
