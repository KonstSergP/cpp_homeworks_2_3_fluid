#include "Simulator.h"

int main(int argc, char* argv[])
{
    auto setts = parseArgs(argc, argv);

    auto sim = Simulator(setts);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; i++)
    {
        sim.nextTick();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Time difference = " << d1 << "[ms]" << std::endl;
}
