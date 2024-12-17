
#include "FieldInfo.h"
#include "SimSetts.h"
#include "Simulator.h"

int main()
{
    auto fi = FieldInfo("../TestField.txt");
    auto st = SimSetts{"../TestField.txt", "../test_out.txt", 1000};

    auto sim = Simulator();
    sim.init(fi, st);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; i++)
    {
        sim.nextTick();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Time difference = " << d1 << "[ms]" << std::endl;

    // предполагается что thread pool-ом управляет один поток

    // int cnt = 0;
    // threadPool tp;
    // auto f = [&cnt](){for (int i = 0; i < 100000000; i++){};cnt++;};
    //
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // f();
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // auto d1 = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    // std::cout << "Time difference = " << d1 << "[µs]" << std::endl;
    //
    // begin = std::chrono::steady_clock::now();
    // for (int i = 0; i < 160; i++)
    // {
    //     tp.addTask(f);
    // }
    // tp.waitForAll();
    // end = std::chrono::steady_clock::now();
    // auto d2 = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    // std::cout << "Time difference = " << d2 << "[µs]" << std::endl;
    // std::cout << (double)d2/d1 << "\n";
    //
    // std::cout << cnt << "\n";
}
