

#include "FieldInfo.h"
#include "SimSetts.h"
#include "Simulator.h"

int main()
{
    auto fi = FieldInfo("../TestField.txt");
    auto st = SimSetts{"../TestField.txt", "../test_out.txt", 100};

    auto sim = Simulator();
    sim.init(fi, st);

    for (int i = 0; i < 1000000; i++)
    {
        sim.nextTick();
    }
}
