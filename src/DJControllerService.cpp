#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {

    const std::string& title = track.get_title();

    // HIT
    if (cache.contains(title)) {
        cache.get(title);
        return 1;
    }
    
    //MISS
    PointerWrapper<AudioTrack> pointer_wrapper_clone = track.clone();
    AudioTrack* track_clone = pointer_wrapper_clone.get();

    if (!track_clone) {
        std::cerr << "[ERROR] Track clone failed for: " << title << "\n";
        return 0; 
    }

    track_clone->load();
    track_clone->analyze_beatgrid();

    //eviction -> return -1
    //no eviction -> return 0
    if (cache.put(std::move(pointer_wrapper_clone)))
        return -1; 
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {    
    AudioTrack* track = cache.get(track_title);
    if (!track) {
        return nullptr;
    }
    return track;
}
