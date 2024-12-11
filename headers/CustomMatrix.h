#pragma once

#include <exception>
#include <iostream>
#include <vector>

template <typename T, size_t Nv, size_t Mv>
struct CustomMatrix
{
    T v[Nv][Mv]{};

    void init(size_t N, size_t M);
    T* operator[](size_t index);
    CustomMatrix& operator=(const CustomMatrix& b);
};

template <typename T>
struct CustomMatrix<T, 0, 0>
{
    std::vector<std::vector<T>> v;

    void init(size_t N, size_t M);
    std::vector<T>& operator[](size_t index);
    CustomMatrix& operator=(const CustomMatrix& b);
};



template <typename T, size_t Nv, size_t Mv>
void CustomMatrix<T, Nv, Mv>::init(size_t N, size_t M) {
    if (N != Nv || M != Mv) {std::cout << "Wrong size of field\n"; throw std::exception();}
}

template <typename T>
void CustomMatrix<T, 0, 0>::init(size_t N, size_t M) {
    v.resize(N, std::vector<T>(M));
}

template <typename T, size_t Nv, size_t Mv>
T* CustomMatrix<T, Nv, Mv>::operator[](size_t index) {
    return v[index];
}

template <typename T>
std::vector<T>& CustomMatrix<T, 0, 0>::operator[](size_t index) {
    return v[index];
}


template <typename T, size_t Nv, size_t Mv>
CustomMatrix<T, Nv, Mv>& CustomMatrix<T, Nv, Mv>::operator=(const CustomMatrix& other)
{
    if(this == &other) {return *this;}
    memcpy(v, other.v, sizeof(v));
    return *this;
}

template <typename T>
CustomMatrix<T, 0, 0>& CustomMatrix<T, 0, 0>::operator=(const CustomMatrix& other)
{
    v = other.v;
    return *this;
}
