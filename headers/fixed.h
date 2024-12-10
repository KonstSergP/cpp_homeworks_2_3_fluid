#pragma once

#include <atomic>
#include <iostream>
#include <limits>
#include "constants.h"


template <size_t N1, size_t K1>
struct Fixed
{
    constexpr Fixed(int v): v(v << K1) {}
    constexpr Fixed(float f): v(f * (1 << K1)) {}
    constexpr Fixed(double f): v(f * (1 << K1)) {}
    constexpr Fixed(): v(0) {}
    template <size_t N2, size_t K2>
    constexpr Fixed(Fixed<N2, K2> f): v((K1>K2)?(f.v << (K1-K2)):(f.v >> (K2-K1))) {}//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    static constexpr Fixed from_raw(int64_t x)
    {
        Fixed ret;
        ret.v = x;
        return ret;
    }

    long long v: N1;

    // std::strong_ordering operator<=>(const Fixed&) const;
    bool operator==(const Fixed&) const = default;

    constexpr operator float() const {return (float)v / ((float) (1 << K1));}
    constexpr operator double() const {return (double)v / ((double) (1 << K1));}
};

template <unsigned N, unsigned K>
static constexpr Fixed inf = Fixed<N, K>::from_raw(std::numeric_limits<int64_t>::max());
template <unsigned N, unsigned K>
static constexpr Fixed eps = Fixed<N, K>::from_raw(deltas.size());


template<size_t N1, size_t K1, size_t N2, size_t K2>
auto operator<=>(const Fixed<N1, K1>& a, const Fixed<N2, K2>& b)
{
    return a.v <=> b.v;
}


template<size_t N1, size_t K1, size_t N2, size_t K2>
auto operator+(Fixed<N1, K1> a, Fixed<N2, K2> b){
    return Fixed<N1, K1>::from_raw(a.v + b.v);
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
auto operator-(Fixed<N1, K1> a, Fixed<N2, K2> b){
    return Fixed<N1, K1>::from_raw((int64_t)a.v - (int64_t)b.v);
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
Fixed<N1, K1> operator*(Fixed<N1, K1> a, Fixed<N2, K2> b){
    return Fixed<N1, K1>::from_raw(((int64_t)a.v * b.v) >> K1);
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
auto operator/(Fixed<N1, K1> a, Fixed<N2, K2> b){
    return Fixed<N1, K1>::from_raw(((int64_t)a.v << K1) / b.v);
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
Fixed<N1, K1>& operator+=(Fixed<N1, K1> &a, Fixed<N2, K2> b) {
    return a = a + b;
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
auto& operator-=(Fixed<N1, K1> &a, Fixed<N2, K2> b) {
    return a = a - b;
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
auto& operator*=(Fixed<N1, K1> &a, Fixed<N2, K2> b) {
    return a = a * b;
}

template<size_t N1, size_t K1, size_t N2, size_t K2>
auto& operator/=(Fixed<N1, K1> &a, Fixed<N2, K2> b) {
    return a = a / b;
}

template<size_t N1, size_t K1>
auto operator-(Fixed<N1, K1> x) {
    return Fixed<N1, K1>::from_raw(-x.v);
}

template<size_t N1, size_t K1>
auto abs(Fixed<N1, K1> x) {
    if (x.v < 0) {
        x.v = -x.v;
    }
    return x;
}

template<size_t N1, size_t K1>
std::ostream &operator<<(std::ostream &out, Fixed<N1, K1> x) {
    return out << x.v / (double) (1 << K1);
}

// template <int N, int K>
// Fixed<N, K> operator+(Fixed a, Fixed b) {
//     return Fixed::from_raw(a.v + b.v);
// }
// Fixed operator-(Fixed a, Fixed b) {
//     return Fixed::from_raw(a.v - b.v);
// }
//
// Fixed operator*(Fixed a, Fixed b) {
//     return Fixed::from_raw(((int64_t) a.v * b.v) >> 16);
// }
//
// Fixed operator/(Fixed a, Fixed b) {
//     return Fixed::from_raw(((int64_t) a.v << 16) / b.v);
// }
//
// Fixed &operator+=(Fixed &a, Fixed b) {
//     return a = a + b;
// }
//
// Fixed &operator-=(Fixed &a, Fixed b) {
//     return a = a - b;
// }
//
// Fixed &operator*=(Fixed &a, Fixed b) {
//     return a = a * b;
// }
//
// Fixed &operator/=(Fixed &a, Fixed b) {
//     return a = a / b;
// }
//
// Fixed operator-(Fixed x) {
//     return Fixed::from_raw(-x.v);
// }
//
// Fixed abs(Fixed x) {
//     if (x.v < 0) {
//         x.v = -x.v;
//     }
//     return x;
// }
//
// std::ostream &operator<<(std::ostream &out, Fixed x) {
//     return out << x.v / (double) (1 << 16);
// }
