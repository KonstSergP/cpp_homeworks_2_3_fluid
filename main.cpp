#include "Simulator.h"

int main()
{
    auto st = SimSetts{"../BigTestField.txt", "../test_out.txt", 1000};

    auto sim = Simulator();
    sim.init(st);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 50; i++)
    {
        sim.nextTick();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Time difference = " << d1 << "[ms]" << std::endl;

}
