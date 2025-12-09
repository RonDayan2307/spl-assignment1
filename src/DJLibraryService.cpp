#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>
#include <algorithm> 

DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    std::cout << "[INFO] Building track library from config...\n";

    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();

    //duplicating the library while handling all types of audiotrack
    for (const auto& info : library_tracks) {
        AudioTrack* new_track = nullptr;

        if (info.type == "MP3") {
            new_track = new MP3Track(info.title, info.artists, info.duration_seconds, info.bpm,
                                     info.extra_param1, info.extra_param2 == 1);
        } else if (info.type == "WAV") {
            new_track = new WAVTrack(info.title, info.artists, info.duration_seconds, info.bpm,
                                     info.extra_param1, info.extra_param2);
        }

        if (new_track) {
            library.push_back(new_track);
        } else {
            std::cerr << "[ERROR] Failed to create track of type: " << info.type << std::endl;
        }
    }

    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    for (AudioTrack* track : library) {
        if (track->get_title() == track_title) {
            return track;
        }
    }
    return nullptr;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";

    playlist = Playlist(playlist_name);
    std::vector<int> indices_copy = track_indices;

    // 2. Reverse the local copy
    std::reverse(indices_copy.begin(), indices_copy.end());
    
    for (int index : track_indices) {
        size_t lib_index = static_cast<size_t>(index - 1);
        if (lib_index >= library.size()) {
            std::cerr << "[WARNING] Invalid track index " << index
                      << " in playlist '" << playlist_name << "'. Skipping.\n";
            continue;
        }

        AudioTrack* original_track = library[lib_index];

        PointerWrapper<AudioTrack> cloned_wrapper = original_track->clone();
        AudioTrack* track_for_playlist = cloned_wrapper.release();

        if (!track_for_playlist) {
            std::cerr << "[ERROR] Track: \"" << original_track->get_title()
                      << "\" failed to clone. Skipping.\n";
            continue;
        }

        track_for_playlist->load();
        track_for_playlist->analyze_beatgrid();
        playlist.add_track(track_for_playlist);
    }

    if (!playlist.is_empty()) {
        std::cout << "[INFO] Playlist loaded: " << playlist_name
                  << " (" << playlist.get_track_count() << " tracks)\n";
    }
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (AudioTrack* track : tracks) {
        titles.push_back(track->get_title());
    }
    return titles;
}
