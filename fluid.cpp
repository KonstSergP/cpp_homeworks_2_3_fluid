#include "headers/Simulator.h"



int main()
{
    Simulator<Fixed, Fixed, Fixed, height, width> sim;

    for (size_t i = 0; i < T; ++i)
    {
        sim.nextTick();
    }
}
