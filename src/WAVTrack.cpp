#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character

}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    return 0.0; // Replace with your implementation
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(nullptr); // Replace with your implementation
}


//constructors:

WAVTrack::WAVTrack(const WAVTrack& other) :
    AudioTrack(other), 
    sample_rate(other.sample_rate), 
    bit_depth(other.bit_depth) {}

WAVTrack& WAVTrack::operator=(const WAVTrack& other) {
    // if this=other nothing needs to be done
    if (this != &other){
        AudioTrack::operator=(other);
        sample_rate = other.sample_rate;
        bit_depth = other.bit_depth; 
    }
    return *this;
}

WAVTrack::WAVTrack(WAVTrack&& other) noexcept:
    AudioTrack(other), 
    sample_rate(other.sample_rate), 
    bit_depth(other.bit_depth) {}

WAVTrack& WAVTrack::operator=(WAVTrack&& other) noexcept {
    // if this=other nothing needs to be done
    if (this != &other){
        AudioTrack::operator=(other);
        sample_rate = other.sample_rate;
        bit_depth = other.bit_depth; 
    }
    return *this;
}