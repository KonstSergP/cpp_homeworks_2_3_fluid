#pragma once

#include <iostream>
#include <limits>
#include "constants.h"


struct Fixed
{
    constexpr Fixed(int64_t v): v(v << 16) {}
    constexpr Fixed(float f): v(f * (1 << 16)) {}
    constexpr Fixed(double f): v(f * (1 << 16)) {}
    constexpr Fixed(): v(0) {}

    static constexpr Fixed from_raw(int32_t x) {
        Fixed ret;
        ret.v = x;
        return ret;
    }

    int32_t v;

    explicit constexpr operator float() const {return (double)v / ((double) (1 << 16));}
    explicit constexpr operator double() const {return (double)v / ((double) (1 << 16));}

    auto operator<=>(const Fixed&) const = default;
    bool operator==(const Fixed&) const = default;
};

static constexpr Fixed inf = Fixed::from_raw(std::numeric_limits<int32_t>::max());
static constexpr Fixed eps = Fixed::from_raw(deltas.size());

Fixed operator+(Fixed a, Fixed b);

Fixed operator-(Fixed a, Fixed b);

Fixed operator*(Fixed a, Fixed b);

Fixed operator/(Fixed a, Fixed b);

Fixed &operator+=(Fixed &a, Fixed b);

Fixed &operator-=(Fixed &a, Fixed b);

Fixed &operator*=(Fixed &a, Fixed b);

Fixed &operator/=(Fixed &a, Fixed b);

Fixed operator-(Fixed x);

Fixed abs(Fixed x);

std::ostream &operator<<(std::ostream &out, Fixed x);
