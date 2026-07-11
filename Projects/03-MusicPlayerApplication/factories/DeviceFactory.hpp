#pragma once
#include <iostream>
#include "../device/IAudioOutputDevice.hpp"
#include "../device/BluetoothAdapter.hpp"
#include "../device/WiredSpeakerAdapter.hpp"
#include "../device/HeadphonesAdapter.hpp"
#include "../enums/DeviceType.hpp"
using namespace std;

class DeviceFactory {
public:
    static IAudioOutputDevice* createDevice(DeviceType deviceType){
        if(deviceType == DeviceType::BLUETOOTH){
            return new BluetoothAdapter(new BluetoothAPI());
        } else if(deviceType == DeviceType::HEADPHONES){
            return new HeadphonesAdapter(new HeadphonesAPI());
        } else { // wired
            return new WiredSpeakerAdapter(new WiredSpeakerAPI());
        }
    }
};