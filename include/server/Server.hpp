#pragma once
#include "storage/Storage.hpp"
#include "server/ThreadPool.hpp"
#include <string>

class Server {
public: 
    Server(int port, Storage& db, size_t thread_count = 4);
    ~Server();
    void start();

private:
    int server_fd;
    int port;
    Storage& db;
    ThreadPool pool;
    
    void handleClient(int client_socket);
};