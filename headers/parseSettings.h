#pragma once

#define FIXED(n, k) (100*n+k)
#define FAST_FIXED(n, k) (10000*n+k)
#define FLOAT 1000000
#define DOUBLE 2000000


struct SimulatorSettings
{
    int p_type = 0, v_type = 0, vf_type = 0;
};



SimulatorSettings parseArgs(int argc, char* argv[]);