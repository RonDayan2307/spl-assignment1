#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
}

MixingEngineService::MixingEngineService(const MixingEngineService &other):
active_deck(other.active_deck),
auto_sync(other.auto_sync),
bpm_tolerance(other.bpm_tolerance)
{
    decks[0] = other.decks[0];
    decks[1] = other.decks[1];
}

MixingEngineService& MixingEngineService:: operator=(const MixingEngineService &other)
{
    if (this != &other){
        active_deck = other.active_deck;
        auto_sync = other.auto_sync;
        bpm_tolerance = other.bpm_tolerance;
        decks[0] = other.decks[0];
        decks[1] = other.decks[1];
    }

    return *this;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    if (decks[0]) delete decks[0];
    if (decks[1]) delete decks[1];
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    int target_deck = 0;
    if (active_deck == 0 ){
        target_deck = 1;
    }

    std::cout << "\n=== Loading Track to Deck ===\n";
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    PointerWrapper<AudioTrack> track_wrapper = track.clone();
    
    if (!track_wrapper) {
        std::cerr << "[ERROR] Failed to clone track for deck loading.\n";
        return -1;
    }

    track_wrapper->load();
    track_wrapper->analyze_beatgrid();

    if (auto_sync) {
        if (decks[active_deck] != nullptr) {
            if (can_mix_tracks(track_wrapper)) {
                sync_bpm(track_wrapper);
            }
        } else {
            std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
        }
    }

    if (decks[target_deck] != nullptr) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    std::cout << "[Load Complete] '" << track_wrapper->get_title() << "' is now loaded on deck " << target_deck << std::endl;

    if (decks[active_deck] != nullptr) {
        std::cout << "[Unload] Unloading previous deck " << active_deck 
                  << " (" << decks[active_deck]->get_title() << ")\n";
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }

    decks[target_deck] = track_wrapper.release();
    active_deck = target_deck;

    std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;

    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    //check for nullptr
    if (!decks[active_deck] || !track) {
        return false;
    }

    int current_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    bool ans = ((current_bpm - new_bpm) <= bpm_tolerance && (new_bpm - current_bpm) <= bpm_tolerance);
    return ans;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    //check for nullptr
    if (!decks[active_deck] || !track) {
        return;
    }
    //average
    int current_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int average_bpm = (current_bpm + new_bpm) / 2;
    //log and set
    std::cout << "[Sync BPM] Syncing BPM from " << new_bpm << " to " << average_bpm << std::endl;
    track.get()->set_bpm(average_bpm);
    return;
}
