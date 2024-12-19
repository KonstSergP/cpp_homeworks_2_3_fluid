
#include "ThreadPool.h"
#include <iostream>
#include <utility>

void task::operator()() {
    f();
}


threadPool::threadPool(unsigned threadCnt): end(false), running(0)
{
    for (unsigned i = 0; i < threadCnt; i++) {
        threads.push_back(std::thread(&threadPool::work, this));
    }
}


threadPool::~threadPool()
{
    end = true;
    for (auto& t : threads) {
        t.join();
    }
}


void threadPool::addTask(task t) {
    tasksQueue.push(std::move(t));
}


void threadPool::work()
{
    while (!end)
    {
        bool worked = false;
        task t;
        while (tasksQueue.tryPop(t)) {
            if (!worked) {
                running.fetch_add(1);
                worked = true;
            }
            t();
        }
        if (worked) running.fetch_sub(1);
    }
}


void threadPool::waitForAll() {
    while (running.load() || !tasksQueue.empty());
}
