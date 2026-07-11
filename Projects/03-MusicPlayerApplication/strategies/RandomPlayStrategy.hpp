#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
#include "../models/Playlist.hpp"
#include "PlayStrategy.hpp"
using namespace std;

class RandomPlayStrategy : public PlayStrategy {
private:
    Playlist* currentPlaylist;
    vector<Song*> remainingSongs;
    stack<Song*> history;

public:
    RandomPlayStrategy() {
        currentPlaylist = nullptr;
        srand((unsigned)time(nullptr));
    }

    void setPlaylist(Playlist* playlist) override {
        currentPlaylist = playlist;
        if(!currentPlaylist || currentPlaylist->getSize() == 0) return;

        remainingSongs = currentPlaylist->getSongs();
        history = stack<Song*>();
    }

    bool hasNext() override {
        return currentPlaylist && !remainingSongs.empty();
    }

    // next song
    Song* next() override {
        if(!currentPlaylist || currentPlaylist->getSize() == 0) throw runtime_error("No playlist loaded or playlist is empty");

        if(remainingSongs.empty()) throw runtime_error("no songs left to play");

        int idx = rand() % remainingSongs.size();
        Song* selectedSong = remainingSongs[idx];

        // remove the seleted song from list
        swap(remainingSongs[idx], remainingSongs.back());
        remainingSongs.pop_back();

        history.push(selectedSong);
        return selectedSong;
    }

    bool hasPrevious() override {
        return history.size() > 0;
    }

    Song* previous() override {
        if(history.empty()) throw runtime_error("No prevous song available.");

        Song* song = history.top();
        history.pop();
        return song;
    }
};
