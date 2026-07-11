#pragma once
#include "../models/Song.hpp"
#include "IAudioOutputDevice.hpp"
#include "../external/BluetoothAPI.hpp"
using namespace std;

class BluetoothAdapter : public IAudioOutputDevice {
private:
    BluetoothAPI* bluetoothApi;
public:
    BluetoothAdapter(BluetoothAPI* api){
        bluetoothApi = api;
    }

    void playAudio(Song* song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        bluetoothApi->playSoundViaBluetooth(payload);
    }
};