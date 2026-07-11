#pragma once
#include <string>
#include <iostream>
using namespace std;

class BluetoothAPI {
public:
    void playSoundViaBluetooth(const string& data){
        cout << "[Bluetooth] playing: " << data << endl;
    }
};