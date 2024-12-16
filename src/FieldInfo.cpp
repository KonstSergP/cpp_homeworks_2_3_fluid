#include <fstream>
#include <iostream>

#include "FieldInfo.h"


FieldInfo::FieldInfo(const std::string &filename) {
    readFromFile(filename);
}

void FieldInfo::readFromFile(const std::string& filename)
{
    std::ifstream in(filename);
    size_t k; uint8_t symbol; double dense; std::string line;
    in >> height >> width >> g >> k; in.get();

    for (size_t i = 0; i < k; i++)
    {
        symbol = in.get();
        in >> dense;
        densities[symbol] = dense;
        in.get();
    }

    for (size_t i = 0; i < height; i++)
    {
        getline(in, line);
        field.emplace_back(line.begin(), line.end());
    }
    in.close();
}
