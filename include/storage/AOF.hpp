#pragma once
#include <string>
#include <fstream>
#include <mutex>

using namespace std;

class AOF {
public:
    AOF(const string& filename); // Must have const string&
    ~AOF();

    void write(const string& cmd); // Must have const string&
    void load(class Storage& db);

private:
    ofstream file;
    string filename;
    mutex file_mutex;
};