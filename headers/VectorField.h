#pragma once

#include <cassert>
#include "constants.h"

template <typename Type, int N, int M>
struct VectorField
{
    std::array<Type, deltas.size()> v[N][M];
    Type& add(int x, int y, int dx, int dy, Type dv) {
        return get(x, y, dx, dy) += dv;
    }

    Type& get(int x, int y, int dx, int dy)
    {
        size_t i = std::ranges::find(deltas, std::pair(dx, dy)) - deltas.begin();
        assert(i < deltas.size());
        return v[x][y][i];
    }
};
