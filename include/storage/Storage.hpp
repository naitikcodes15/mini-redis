#include "storage/AOF.hpp"
#pragma once
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include <list>
#include<memory>

using namespace std;

class Storage {
public:
    Storage(size_t capacity = 100); 
    ~Storage() = default;

    void set(const string& key, const string& value, bool log = true);
    optional<string> get(const string& key);
    bool del(const string& key);

    void setAOF(shared_ptr<AOF> aof_ptr) {
    persistence = aof_ptr;
}

private:
    size_t capacity;
    shared_ptr<AOF> persistence;
    
    unordered_map<string, pair<string, list<string>::iterator>> data_store;
    
    list<string> lru_list;
    
    mutable shared_mutex rw_mutex;

    void evict(); 
};