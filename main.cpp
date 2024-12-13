#include <memory>

#include "headers/Simulator.h"
#include "headers/parseSettings.h"
#include "headers/TypeGeneration.h"



constexpr auto simulators = generateSimulators();
constexpr auto types      = generateTypes();

// здесь имеет смысл описать почему функций 4 а не 2 (на заметку)


int main(int argc, char* argv[])
{
    SimSetts st = parseArgs(argc, argv);
    FieldInfo info(st.input_filename);

    tuple need = {st.p_type, st.v_type, st.vf_type, info.height, info.width};
    auto index = std::find(types.begin(), types.end(), need) - types.begin();
    if (index == types.size())
    {
        need = {st.p_type, st.v_type, st.vf_type, 0, 0};
        index = std::find(types.begin(), types.end(), need) - types.begin();
    }
    if (index == types.size()) {
        std::cout << "Simulator with chosen types does not exist\n"; exit(EXIT_FAILURE);
    }

    auto sim = simulators[index]();
    sim->init(info, st);

    for (size_t i = 0; i < 1000000; ++i) {
        sim->nextTick();
    }
}
