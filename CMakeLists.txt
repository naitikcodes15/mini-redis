cmake_minimum_required(VERSION 3.10)
project(MiniRedis VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Find threads for concurrency
find_package(Threads REQUIRED)

#Include Directories
include_directories(include)

#Add executable
add_executable(mini-redis 
    src/main.cpp 
    src/server/Server.cpp 
    src/storage/Storage.cpp
    src/protocol/Parser.cpp
    src/server/ThreadPool.cpp
    src/storage/AOF.cpp
)

#Link threads
target_link_libraries(mini-redis PRIVATE Threads::Threads)
