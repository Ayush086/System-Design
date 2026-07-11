#pragma once
#include <string>
#include <iostream>
using namespace std;

class WiredSpeakerAPI {
public:
    void playSoundViaCable(const string& data){
        cout << "[Wired Speaker] playing: " << data << endl;
    }
};