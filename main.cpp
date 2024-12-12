#include <memory>

#include "headers/Fixed.h"
#include "headers/Simulator.h"

using std::conditional_t, std::array;

#define FIXED(n, k) (100*n+k)
#define FAST_FIXED(n, k) (10000*n+k)
#define FLOAT 1000000
#define DOUBLE 2000000

#ifndef SIZES
#define SIZES
#endif
#define S(a, b) pair<int, int>(a, b)

constexpr array t{TYPES};
constexpr array s{ pair{0, 0}, SIZES};

template <int num>
struct stNumberToType {using type = conditional_t<(num==FLOAT), float,
                                    conditional_t<(num==DOUBLE), double,
                                    conditional_t<(num < 10000), Fixed<num/100,num%100>, FastFixed<num/10000,num%10000>>>>;};

template <int num>
using numType = typename stNumberToType<num>::type;


template <typename P, typename V, typename VF, size_t N, size_t M>
constexpr std::unique_ptr<Simulator> generate()
{
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
constexpr auto simulatorsGenerator<t.size()*t.size()*t.size()*s.size()>()
{
    return array<genfunc, t.size()*t.size()*t.size()*s.size()>();
}

constexpr auto funcs = simulatorsGenerator<0>();



int main()
{
    // for (auto b: t)
    // {
    //     std::cout << b << "\n";
    // }
    // Simulator<Fixed<32, 16>, double, FastFixed<48, 20>, 24, 84> sim1;
    //
    // FieldInfo info("../TestField.txt");
    //
    // sim1.init(info);
    //
    // for (size_t i = 0; i < 1000000; ++i)
    // {
    //     sim1.nextTick();
    // }
}
