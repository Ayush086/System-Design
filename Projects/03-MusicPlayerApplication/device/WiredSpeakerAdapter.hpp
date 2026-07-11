#pragma once
#include "../models/Song.hpp"
#include "IAudioOutputDevice.hpp"
#include "../external/WiredSpeakerAPI.hpp"
using namespace std;

class WiredSpeakerAdapter : public IAudioOutputDevice {
private:
    WiredSpeakerAPI* wiredSpeakerApi;
public:
    WiredSpeakerAdapter(WiredSpeakerAPI* api){
        wiredSpeakerApi = api;
    }

    void playAudio(Song* song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        wiredSpeakerApi->playSoundViaCable(payload);
    }
};