#pragma once
#include "storage/Storage.hpp"
#include "server/ThreadPool.hpp"
#include <string>

class Server {
public: 
    // Added thread_count parameter with a default value of 4
    Server(int port, Storage& db, size_t thread_count = 4);
    ~Server();
    void start();

private:
    int server_fd;
    int port;
    Storage& db;
    ThreadPool pool; // The ThreadPool instance
    
    void handleClient(int client_socket);
};