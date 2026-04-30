#include "server/Server.hpp"
#include "protocol/Parser.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

// Initialize the pool in the constructor's initializer list
Server::Server(int port, Storage& storage, size_t thread_count) 
    : port(port), db(storage), server_fd(-1), pool(thread_count) {}

Server::~Server() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

void Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        cerr << "Failed to create socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed\n";
        return;
    }

    if (listen(server_fd, 10) < 0) {
        cerr << "Listen failed\n";
        return;
    }
    
    cout << "Server listening on port " << port << " with " << 4 << " threads...\n";

    while (true) {
        sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);

        int client_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
        
        if (client_socket >= 0) {
            // NEW: Instead of handleClient(client_socket), we wrap it in a lambda 
            // and push it into the thread pool's task queue.
            pool.enqueue([this, client_socket]() {
                this->handleClient(client_socket);
            });
        }
    }
}

void Server::handleClient(int client_socket) {
    char buffer[1024] = {0};
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (bytes_read > 0) {
        Command cmd = Parser::parse(string(buffer));
        string response;

        if (cmd.type == CommandType::SET && cmd.args.size() == 2) {
            db.set(cmd.args[0], cmd.args[1]);
            response = "+OK\r\n";
        } 
        else if (cmd.type == CommandType::GET && cmd.args.size() == 1) {
            auto val = db.get(cmd.args[0]);
            response = val.has_value() ? 
                       "$" + to_string(val->length()) + "\r\n" + *val + "\r\n" : 
                       "$-1\r\n";
        }
        else if (cmd.type == CommandType::PING) {
            response = "+PONG\r\n";
        }
        else {
            response = "-ERR unknown command or wrong arguments\r\n";
        }

        send(client_socket, response.c_str(), response.length(), 0);
    }
    close(client_socket);
}