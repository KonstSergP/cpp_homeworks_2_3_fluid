#pragma once

#include <utility>

#include "FieldInfo.h"

struct SimSetts
{
    std::string input_filename, output_filename;
    int64_t n_ticks;
    FieldInfo info;

    SimSetts(const std::string& in, std::string  out, const int n_ticks): input_filename(in), output_filename(std::move(out)),
                                                                            n_ticks(n_ticks), info(in)
    {}
};
