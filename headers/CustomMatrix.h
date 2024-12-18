#pragma once

#include <cassert>
#include <cstring>

template <typename T>
struct CustomMatrix
{
    size_t N, M;
    T** v;

    void init(size_t N, size_t M);
    T* operator[](size_t index);
    CustomMatrix& operator=(const CustomMatrix& b);
    void clear();
    ~CustomMatrix();
};


template <typename T>
void CustomMatrix<T>::init(size_t Nv, size_t Mv) {
    N = Nv; M = Mv;
    v = new T*[N];
    for (int i = 0; i < N; i++) {
        v[i] = new T[M];
    }
}


template <typename T>
T* CustomMatrix<T>::operator[](size_t index) {
    return v[index];
}


template <typename T>
CustomMatrix<T>& CustomMatrix<T>::operator=(const CustomMatrix& other)
{
    assert(N == other.N && M == other.M);
    if(this == &other) {return *this;}
    for (int i = 0; i < N; i++) {
        memcpy(v[i], other.v[i], M * sizeof(T));
    }
    return *this;
}


template <typename T>
CustomMatrix<T>::~CustomMatrix()
{
    for (int i = 0; i < N; i++) {
        delete[] v[i];
    }
    delete[] v;
}

template <typename T>
void CustomMatrix<T>::clear()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            v[i][j] = T{};
        }
    }
}
