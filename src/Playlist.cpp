#include "Playlist.h"
#include "AudioTrack.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_count(0) {
    std::cout << "Created playlist: " << name << std::endl;
}
// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    while (head != nullptr) {
        PlaylistNode* next = head->next;
        delete head -> track;
        delete head;
        head = next;
    }
    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif
}

Playlist::Playlist(const Playlist& other) 
    : head(nullptr), playlist_name(other.playlist_name), track_count(0) {
    
    std::cout << "Copying playlist: " << playlist_name << std::endl;

    PlaylistNode* src_node = other.head;
    PlaylistNode** dst_ptr = &head; 

    while (src_node) {
        if (src_node->track) {
            PointerWrapper<AudioTrack> cloned_wrapper = src_node->track->clone();
            AudioTrack* new_track = cloned_wrapper.release();
            if (new_track) {
                *dst_ptr = new PlaylistNode(new_track);
                dst_ptr = &((*dst_ptr)->next);
                track_count++;
            }
        }
        src_node = src_node->next;
    }
}

Playlist& Playlist::operator=(const Playlist& other) {
    if (this != &other) {
        Playlist temp(other);
        swap(temp);
    }
    return *this;
}

Playlist::Playlist(Playlist&& other) noexcept 
    : head(nullptr), playlist_name(""), track_count(0) {
    swap(other);
    #ifdef DEBUG
    std::cout << "Playlist moved: " << playlist_name << std::endl;
    #endif
}

Playlist& Playlist::operator=(Playlist&& other) noexcept {
    if (this != &other) {
        while (head != nullptr) {
            PlaylistNode* next = head->next;
            delete head->track;
            delete head;
            head = next;
        }
        head = nullptr;
        track_count = 0;
        
        swap(other);
    }
    return *this;
}

void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_count++;

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }

        delete current->track;
        current->next = nullptr;
        delete current;

        track_count--;
        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }
}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_count << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_count == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}

AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}

void Playlist::swap(Playlist& other) noexcept {
    std::swap(playlist_name, other.playlist_name);
    std::swap(head, other.head);
    std::swap(track_count, other.track_count);
}