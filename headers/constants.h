#pragma once

#include <array>

constexpr size_t T = 1'000'000;
constexpr std::array<std::pair<int, int>, 4> deltas{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
