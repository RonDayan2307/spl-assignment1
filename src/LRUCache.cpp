#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    bool ans = false;

    //handling 4 scenarios:
    //1 - track is empty
    //2 - track exists in cache already therefore it will be updated
    //3 - track does not exist and the cache is full - eviction of LRU
    //4 - track does not exist and the cache is not full - no eviction

    //1
    if(!track){
        return false;
    }

    //2
    const std::string& new_title = track->get_title();

    size_t track_index = findSlot(new_title);

    for (size_t i = 0; i < max_size; ++i) {
        if (track_index != max_size) {
            access_counter++;
            slots[track_index].access(access_counter);
            return false;
        }
    }

    //3 - if full then evacuate
    size_t empty_slot = findEmptySlot();
    if (empty_slot == max_size) {
        evictLRU();
        ans = true;
        empty_slot = findEmptySlot();
    }

    //4 + continue 3 - update cache
    access_counter++;
    slots[empty_slot].store(std::move(track), access_counter);

    return ans; 
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    int ans = max_size;
    int time = -1;
    for (size_t i = 0; i < max_size; i++) {
        if (slots[i].isOccupied()) {
            int value = slots[i].getLastAccessTime();
            if (time > value || time == -1){
                time = value;
                ans = i;
            }
        }
    }
    return ans; 
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}