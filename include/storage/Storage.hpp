#include "storage/AOF.hpp"
#pragma once
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include <list> // Added for LRU tracking
#include<memory>

using namespace std;

class Storage {
public:
    // Capacity defines when eviction starts
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
    
    // Map stores: key -> {value, iterator_to_list_node}
    unordered_map<string, pair<string, list<string>::iterator>> data_store;
    
    // List stores: keys (Front = Most Recent, Back = Oldest)
    list<string> lru_list;
    
    mutable shared_mutex rw_mutex;

    // Internal helper to drop the oldest item
    void evict(); 
};