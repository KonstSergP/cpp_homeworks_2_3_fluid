#include "headers/Simulator.h"



int main()
{
    Simulator<Fixed<32, 16>, Fixed<32, 16>, Fixed<32, 16>, height, width> sim;

    for (size_t i = 0; i < T; ++i)
    {
        sim.nextTick();
    }

}
