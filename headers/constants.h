#pragma once

#include <array>
#include <cstddef>

constexpr size_t height = 36, width = 84;
constexpr size_t T = 1'000'000;
constexpr std::array<std::pair<int, int>, 4> deltas{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};


extern char field[height][width + 1];
