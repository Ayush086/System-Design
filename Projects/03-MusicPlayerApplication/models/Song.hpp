#pragma once
#include <string>
#include <iostream>
using namespace std;

class Song{
private:
    string title;
    string artist;
    string filePath;
public:
    Song(string t, string a, string path){
        this->title = t;
        this->artist = a;
        this->filePath = path;
    }

    // getter nd setters
    string getTitle() {
        return title;
    }

    string getArtist() {
        return artist;
    }

    string getFilePath() {
        return filePath;
    }
};