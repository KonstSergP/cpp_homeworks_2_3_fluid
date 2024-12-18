#pragma once

#include <array>
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


    // высоконагруженная часть, надо описать логику работы
    // эта функция работает на 1-2% медленней swich case
    // Fixed &get(int x, int y, int dx, int dy) {
    //     return v[x][y][((dy&1)<<1) | (((dx&1)&((dx&2)>>1))|((dy&1)&((dy&2)>>1)))];
    // }
    // "оптимизированная" функция ниже плод совместных трудов Петрова Константина, Чубенко Семена, Сокурова Идара
    // и является логическим продолжением верхней
    Fixed &get(int x, int y, int dx, int dy) {
        switch ((dx<<2) + dy)
        {
            case -1:
                return v[x][y][0];
            case 1:
                return v[x][y][1];
            case -4:
                return v[x][y][2];
            case 4:
                return v[x][y][3];
            default:
                throw;
        }
    }


    void clear() {
        v.clear();
    }
};
