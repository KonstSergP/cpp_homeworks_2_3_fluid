#pragma once

#include <cstddef>
#include <cstdint>
#include <random>

#include "VectorField.h"
#include "Fixed.h"
#include "FieldInfo.h"
#include "SimSetts.h"

struct Simulator
{
    VectorField velocity{};
    VectorField velocity_flow{};
    CustomMatrix<Fixed>  p{}, old_p{};
    CustomMatrix<int64_t> last_use{}, dirs{};
    CustomMatrix<uint8_t> field{};

    size_t N, M;
    Fixed rho[256]{};
    Fixed g{};
    int64_t UT = 0;
    std::mt19937 rnd;
    int64_t n_ticks{}, cur_tick{}; std::string out_name;

    Simulator();

    std::tuple<Fixed, bool, std::pair<int, int>> propagate_flow(int x, int y, Fixed lim);
    void propagate_stop(int x, int y, bool force = false);
    Fixed move_prob(int x, int y);
    void swap_between(int x, int y, int nx, int ny);
    bool propagate_move(int x, int y, bool is_first);
    Fixed random01();
    void directionsInit();
    void nextTick();
    void init(const FieldInfo& f, const SimSetts& setts);
    void serialize();
};
