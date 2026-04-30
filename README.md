# Mini-Redis

A high-performance, multi-threaded, custom key-value store built from scratch in C++. This project is a lightweight, simplified clone of Redis, designed to demonstrate core systems engineering concepts including network programming, concurrency, custom protocol parsing, and data persistence.

## 🚀 Features & Architecture

- **Multi-threaded TCP Server**: Implements a robust socket listener handling concurrent client connections using a custom **Thread Pool** to prevent thread exhaustion and blocking.
- **Thread-Safe Storage Engine**: Utilizes `std::unordered_map` protected by **Reader-Writer Locks** (`std::shared_mutex`) to allow high-throughput concurrent reads and safe exclusive writes.
- **Protocol Parsing**: A custom command parser capable of deserializing raw string streams (e.g., `SET key value`) into actionable server commands.
- **LRU Cache Eviction**: Intelligent memory management incorporating a **Doubly Linked List** and Hash Map to track and evict the Least Recently Used items when the memory capacity is reached.
- **Disk Persistence (AOF)**: Ensures data reliability by logging every write operation to an **Append-Only File (AOF)**. Automatically recovers and rebuilds state upon server restart.

## 🛠 Tech Stack

- **Language**: C++ (Modern C++17)
- **Build System**: CMake
- **Networking**: POSIX Sockets (TCP/IP)
- **Concurrency**: `std::thread`, `std::mutex`, `std::shared_mutex`, `std::condition_variable`
- **Data Structures**: `std::unordered_map`, `std::list` (Doubly Linked List)

## 🧠 Engineering Skills Demonstrated

- **Low-Level System Design**: Building a client-server architecture from the ground up without relying on heavy web frameworks.
- **Concurrency & Synchronization**: Managing race conditions, preventing deadlocks, and optimizing throughput with Reader-Writer locks and Thread Pools.
- **Memory Management & Data Structures (DSA)**: Combining hash maps and doubly linked lists for O(1) LRU Cache operations.
- **Network Programming**: Handling raw socket I/O, byte streams, and custom protocol serialization/deserialization.
- **Durability & Fault Tolerance**: Implementing logging and recovery mechanisms (AOF) to ensure system reliability across reboots.

## ⚙️ Setup & Installation

### Prerequisites
- A C++17 compatible compiler (e.g., GCC, Clang)
- CMake (version 3.10 or higher)
- Make (or another build system)

### Building the Project

1. **Clone the repository** (if applicable) or navigate to the project directory:
   ```bash
   cd mini-redis
   ```

2. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```

3. **Generate build files with CMake**:
   ```bash
   cmake ..
   ```

4. **Compile the project**:
   ```bash
   make
   ```

### Running the Server

Start the compiled executable from the build directory:
```bash
./mini-redis
```
*The server will start listening on port 6379 by default.*

### Connecting a Client

You can interact with the server using standard tools like `nc` (netcat) or `telnet` in a separate terminal:
```bash
nc localhost 6379
```
*Try commands like: `PING`, `SET key value`, `GET key`.*

## 📚 Technical Concepts Reference

For recruiters and developers reviewing this repository, the following core computer science and engineering concepts are heavily utilized:

- **TCP/IP Sockets**: For reliable, stream-oriented networking.
- **Thread Pool Pattern**: For efficient reuse of worker threads and managing multiple connections simultaneously.
- **Reader-Writer Locks (`std::shared_mutex`)**: Allowing multiple threads to read data simultaneously, while writes get exclusive access.
- **LRU (Least Recently Used) Caching**: Implemented using a combination of a Doubly Linked List and a Hash Map to achieve O(1) access and eviction.
- **Append-Only File (AOF)**: A persistence strategy that logs every modifying command sequentially to disk for crash recovery.
- **Protocol Parsing / State Machines**: Parsing raw byte streams into structured commands safely.
