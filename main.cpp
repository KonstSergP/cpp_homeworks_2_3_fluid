#include "headers/Simulator.h"
#include "headers/Fixed.h"




int main()
{
    Simulator<Fixed<32, 16>, double, FastFixed<48, 20>, 24, 84> sim1;

    FieldInfo info("../TestField.txt");

    sim1.init(info);

    for (size_t i = 0; i < 1000000; ++i)
    {
        sim1.nextTick();
    }
}
