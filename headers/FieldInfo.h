#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct FieldInfo
{
    size_t height{};
    size_t width{};
    double densities[256]{}, g{};
    std::vector<std::vector<uint8_t>> field;

    FieldInfo() = default;
    explicit FieldInfo(const std::string& filename);
    void readFromFile(const std::string& filename);
};

