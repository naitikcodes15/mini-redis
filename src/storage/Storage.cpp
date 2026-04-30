#include "storage/Storage.hpp"
#include <mutex>

Storage::Storage(size_t cap) : capacity(cap) {}

optional<string> Storage::get(const string& key) {
    // We need a unique_lock here because moving list nodes is a 'write' operation
    unique_lock<shared_mutex> lock(rw_mutex);
    
    auto it = data_store.find(key);
    if (it == data_store.end()) {
        return nullopt; 
    }

    // LRU Logic: Move the accessed key to the front of the list
    lru_list.erase(it->second.second);
    lru_list.push_front(key);
    it->second.second = lru_list.begin();

    return it->second.first;
}

void Storage::set(const string& key, const string& value, bool log) {
    unique_lock<shared_mutex> lock(rw_mutex);

    auto it = data_store.find(key);
    if (it != data_store.end()) {
        lru_list.erase(it->second.second);
    } else if (data_store.size() >= capacity) {
        evict();
    }

    lru_list.push_front(key);
    data_store[key] = {value, lru_list.begin()};

    // Only write to AOF if log is true and persistence is initialized
    if (log && persistence) {
        persistence->write("SET " + key + " " + value);
    }
}

bool Storage::del(const string& key) {
    unique_lock<shared_mutex> lock(rw_mutex);
    
    auto it = data_store.find(key);
    if (it != data_store.end()) {
        // Phase 6: Log the deletion before removing from memory
        if (persistence) {
            persistence->write("DEL " + key);
        }

        lru_list.erase(it->second.second);
        data_store.erase(it);
        return true;
    }
    
    return false;
}
void Storage::evict() {
    // No lock needed here as it's called inside locked functions
    if (lru_list.empty()) return;

    string oldest_key = lru_list.back();
    lru_list.pop_back();
    data_store.erase(oldest_key);
}