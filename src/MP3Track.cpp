#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    // TODO: Implement MP3 loading with format-specific operations
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement MP3-specific beat detection analysis
    // NOTE: Use exactly 2 spaces before each arrow (→) character

}

double MP3Track::get_quality_score() const {
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything

    return 0.0; // Replace with your implementation
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    // TODO: Implement polymorphic cloning
    return PointerWrapper<AudioTrack>(nullptr); // Replace with your implementation
}


//constructors:

MP3Track::MP3Track(const MP3Track& other) :
    AudioTrack(other), 
    bitrate(other.bitrate), 
    has_id3_tags(other.has_id3_tags) {}

MP3Track& MP3Track::operator=(const MP3Track& other) {
    // if this=other nothing needs to be done
    if (this != &other){
        AudioTrack::operator=(other);
        bitrate = other.bitrate;
        has_id3_tags = other.has_id3_tags; 
    }
    return *this;
}

MP3Track::MP3Track(MP3Track&& other) noexcept:
    AudioTrack(other), 
    bitrate(other.bitrate), 
    has_id3_tags(other.has_id3_tags) {}

MP3Track& MP3Track::operator=(MP3Track&& other) noexcept {
    // if this=other nothing needs to be done
    if (this != &other){
        AudioTrack::operator=(other);
        bitrate = other.bitrate;
        has_id3_tags = other.has_id3_tags; 
    }
    return *this;
}