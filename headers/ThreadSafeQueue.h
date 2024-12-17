#pragma once

#include <queue>
#include <mutex>


template <typename T>
struct threadSafeQueue
{
    bool tryPop(T& item);
    bool empty();
    void push(T item);

    std::mutex mutex;
    std::queue<T> queue;
};


template<typename T>
bool threadSafeQueue<T>::tryPop(T& val)
{
    std::lock_guard lock(mutex);
    if (queue.empty()) {
        return false;
    }
    val = std::move(queue.front());
    queue.pop();
    return true;
}


template<typename T>
void threadSafeQueue<T>::push(T val)
{
    std::lock_guard lock(mutex);
    queue.push(std::move(val));
}


template<typename T>
bool threadSafeQueue<T>::empty()
{
    std::lock_guard lock(mutex);
    return queue.empty();
}
