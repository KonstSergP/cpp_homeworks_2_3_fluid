#pragma once

#include <array>
#include <cassert>
#include "Fixed.h"
#include "CustomMatrix.h"


struct VectorField
{
    CustomMatrix<std::array<Fixed, deltas.size()>> v;

    VectorField() = default;
    void init(int n, int m) {
        v.init(n, m);
    }

    Fixed &add(int x, int y, int dx, int dy, Fixed dv) {
        return get(x, y, dx, dy) += dv;
    }

    Fixed &get(int x, int y, int dx, int dy) {
        size_t i = std::ranges::find(deltas, std::pair(dx, dy)) - deltas.begin();
        assert(i < deltas.size());
        return v[x][y][i];
    }

    void clear() {
        v.clear();
    }
};
