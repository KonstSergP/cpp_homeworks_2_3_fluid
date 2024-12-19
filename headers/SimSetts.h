#pragma once

#include <utility>

#include "FieldInfo.h"

struct SimSetts
{
    std::string input_filename, output_filename;
    int64_t n_ticks=0, n_threads=1;
    FieldInfo info;

    SimSetts() = default;
    SimSetts(const std::string& in, std::string out, const int n_ticks, const int thr): input_filename(in), output_filename(std::move(out)),
                                                                                        n_ticks(n_ticks), n_threads(thr), info(in)
    {}
};

SimSetts parseArgs(int argc, char* argv[]);
