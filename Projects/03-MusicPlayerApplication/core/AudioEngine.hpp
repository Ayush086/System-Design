#pragma once
#include "../models/Song.hpp"
#include "../device/IAudioOutputDevice.hpp"
#include <string>
#include <iostream>
using namespace std;

class AudioEngine{
private:
    Song* currentSong;
    bool songIsPaused;
public:
    AudioEngine() {
        currentSong = nullptr;
        songIsPaused = false;
    }

    // getters and setters
    string getCurrentSongTitle() const {
        if(currentSong) return currentSong->getTitle();
        return "";
    }

    // functionalities
    bool isPaused() const {
        return songIsPaused;
    }

    void play(IAudioOutputDevice* outputDevice, Song* song) {
        if(song == nullptr){
            throw runtime_error("can't play null song");
        }

        // resume song, if paused
        if(songIsPaused && song == currentSong){
            songIsPaused = false;
            cout << "Resuming song: " << song->getTitle() << endl;
            outputDevice->playAudio(song);
            return; 
        }

        // play new song
        currentSong = song;
        songIsPaused = false;
        cout << "Playing song: " << song->getTitle() << endl;
        outputDevice->playAudio(song);
    }

    void pause() {
        if(currentSong == nullptr){
            throw runtime_error("No song is currently playing to pause");
        }
        if(songIsPaused){
            throw runtime_error("Song is already paused");
        }
        songIsPaused = true;
        cout << "Pausing song: " << currentSong->getTitle() << endl;
    }
};