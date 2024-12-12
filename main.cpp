#include <memory>

#include "headers/Fixed.h"
#include "headers/Simulator.h"
#include "headers/parseSettings.h"

using std::conditional_t, std::array;


#ifndef SIZES
#define SIZES
#endif
#define S(a, b) pair<int, int>(a, b)

constexpr array t{TYPES};
constexpr array s{ pair{0, 0}, SIZES};


template <int num>
struct stNumberToType
{
    static auto get()
    {
        if constexpr (num == FLOAT) {
            return std::type_identity<float>{};
        } else if constexpr (num == DOUBLE) {
            return std::type_identity<double>{};
        } else if constexpr (num < 10000) {
            return std::type_identity<Fixed<num/100,num%100>>{};
        } else {
            return std::type_identity<FastFixed<num/10000,num%10000>>{};
        }
    }
};

template <int num>
using numType = typename decltype(stNumberToType<num>::get())::type;


template <typename P, typename V, typename VF, size_t N, size_t M>
constexpr std::unique_ptr<Simulator> generate() {
    return std::make_unique<SimulatorImpl<P, V, VF, N, M>>();
}

using genfunc = std::unique_ptr<Simulator>(*)();


template <int index>
constexpr auto simulatorsGenerator()
{
    auto res = simulatorsGenerator<index+1>();
    res[index] = generate<numType<t[index/(t.size()*t.size()*s.size())]>, numType<t[index%(t.size()*t.size()*s.size())/(t.size()*s.size())]>, numType<t[index%(t.size()*s.size())/s.size()]>, s[index%s.size()].first, s[index%s.size()].second>;
    return res;
}

template <>
constexpr auto simulatorsGenerator<t.size()*t.size()*t.size()*s.size()>() {
    return array<genfunc, t.size()*t.size()*t.size()*s.size()>();
}

constexpr auto funcs = simulatorsGenerator<0>();




template <int index>
constexpr auto typesGenerator()
{
    auto res = typesGenerator<index+1>();
    res[index] = {t[index/(t.size()*t.size()*s.size())], t[index%(t.size()*t.size()*s.size())/(t.size()*s.size())], t[index%(t.size()*s.size())/s.size()], s[index%s.size()].first, s[index%s.size()].second};
    return res;
}

template <>
constexpr auto typesGenerator<t.size()*t.size()*t.size()*s.size()>() {
    return array<tuple<int, int, int, size_t, size_t>, t.size()*t.size()*t.size()*s.size()>();
}

constexpr auto types = typesGenerator<0>();

// здесь имеет смысл описать почему функций 4 а не 2 (на заметку)


int main(int argc, char* argv[])
{
    auto a = parseArgs(argc, argv);
    std::cout << a.p_type << " " << a.v_type << " " << a.vf_type << "\n";
    FieldInfo info("../TestField.txt");

    tuple need = {a.p_type, a.v_type, a.vf_type, info.height, info.width};
    auto index = std::find(types.begin(), types.end(), need) - types.begin();
    std::cout << index << " " << types.size() << "\n";

    // need = {a.p_type, a.v_type, a.vf_type, 0, 0};
    // index = std::find(types.begin(), types.end(), need) - types.begin();
    // std::cout << index << " " << types.size() << "\n";

    auto sim = funcs[index]();
    sim->init(info);

    for (size_t i = 0; i < 1000000; ++i)
    {
        sim->nextTick();
    }
}
