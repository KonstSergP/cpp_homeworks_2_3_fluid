#pragma once

#include <cassert>
#include <array>
#include "constants.h"
#include "CustomMatrix.h"

template <typename Type, int Nv, int Mv>
struct VectorField
{
    size_t N = Nv, M = Mv;
    CustomMatrix<std::array<Type, deltas.size()>, Nv, Mv> v;

    Type& add(int x, int y, int dx, int dy, Type dv) {
        return get(x, y, dx, dy) += dv;
    }


    Type& get(int x, int y, int dx, int dy)
    {
        size_t i = std::ranges::find(deltas, std::pair(dx, dy)) - deltas.begin();
        assert(i < deltas.size());
        return v[x][y][i];
    }

    void clear();
    void init(size_t Nval, size_t Mval);
};

template <typename Type, int Nv, int Mv>
void VectorField<Type, Nv, Mv>::clear()
{
    for (size_t x = 0; x < N; x++) {
        for (size_t y = 0; y < M; y++) {
            for (size_t z = 0; z < deltas.size(); z++)
            {
                v[x][y][z] = Type();
            }
        }
    }
}

template <typename Type, int Nv, int Mv>
void VectorField<Type, Nv, Mv>::init(size_t Nval, size_t Mval)
{
    N = Nval; M = Mval; v.init(Nval, Mval);
}