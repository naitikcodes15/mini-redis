#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    // Adds a new client socket to the queue
    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

    // This is the function each worker thread runs
    void worker_loop();
    
    // We'll need a way to actually handle the client inside the thread
    // We'll pass a reference to the handler later or handle it in the loop
};