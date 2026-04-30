#include "server/ThreadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] { this->worker_loop(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::worker_loop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            
            if (stop && tasks.empty()) return;

            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}