#include "headers/Simulator.h"



int main()
{
    Simulator<Fixed<32, 16>, Fixed<32, 8>, double, height, width> sim;

    for (size_t i = 0; i < T; ++i)
    {
        sim.nextTick();
    }

    // Fixed<31, 16> a = 101;
    // std::cout << a << " " << Fixed<32, 8>(a) << " " << Fixed<16, 8>(a) << " " << Fixed<24, 16>(a) << " " << Fixed<24, 8>(a) << "\n";

}
