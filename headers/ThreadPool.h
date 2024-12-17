#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include "ThreadSafeQueue.h"


struct task
{
    task() = default;
    template <typename F> requires (std::is_invocable_v<F>)
    task(F f) : f(f) {}

    void operator()();
    std::function<void()> f;
};


struct threadPool
{
private:
    void work();

public:

    threadPool(unsigned threadCnt = std::thread::hardware_concurrency());
    ~threadPool();

    void addTask(task t);
    void waitForAll();

    std::atomic_bool end;
    std::atomic_char running;
    threadSafeQueue<task> tasksQueue;
    std::vector<std::thread> threads;
};
