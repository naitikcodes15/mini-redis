#include "storage/AOF.hpp"
#include "storage/Storage.hpp"
#include <sstream>
#include <algorithm>

using namespace std;

// The signature here MUST match the header exactly
AOF::AOF(const string& filename) : filename(filename) {
    file.open(filename, ios::app);
}

AOF::~AOF() {
    if (file.is_open()) file.close();
}

void AOF::write(const string& cmd) {
    lock_guard<mutex> lock(file_mutex);
    if (file.is_open()) {
        file << cmd << endl;
        file.flush(); // Ensure it hits the disk immediately
    }
}

void AOF::load(Storage& db) {
    ifstream read_file(filename);
    if (!read_file.is_open()) return;

    string line;
    while (getline(read_file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string cmd, key, val;
        ss >> cmd >> key;
        
        // Make command uppercase
        transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
        
        if (cmd == "SET") {
            ss >> val;
            db.set(key, val, false); // Load without re-writing to file
        } else if (cmd == "DEL") {
            db.del(key);
        }
    }
}