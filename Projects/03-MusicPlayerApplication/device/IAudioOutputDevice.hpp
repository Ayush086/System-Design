#pragma once
#include "../models/Song.hpp"

// abstract class
class IAudioOutputDevice{
public:
    virtual ~IAudioOutputDevice() {}
    virtual void playAudio(Song* song) = 0;
};