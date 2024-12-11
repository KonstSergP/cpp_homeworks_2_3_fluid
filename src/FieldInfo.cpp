#include "../headers/FieldInfo.h"

#include <fstream>
#include <iostream>

FieldInfo::FieldInfo(const std::string &filename) {
    readFromFile(filename);
}

void FieldInfo::readFromFile(const std::string& filename)
{
    std::ifstream in(filename);
    size_t k; uint8_t symbol; double dense; std::string line;
    in >> height >> width >> g >> k;

    for (size_t i = 0; i < k; i++)
    {
        in >> symbol >> dense;
        densities[symbol] = dense;
    }
    densities[' '] = 0.01;
    in.get(); // newline symbol read

    for (size_t i = 0; i < height; i++)
    {
        getline(in, line);
        field.emplace_back(line.begin(), line.end());
    }
}
