#include "MusicPlayerApplication.hpp"
using namespace std;

int main(){
    try {
        auto application = MusicPlayerApplication::getInstance();

        // songs
        application->createSongInLibrary("Cold", "NEFFEX", "/music/neffex/cold.mp3");
        application->createSongInLibrary("New Beginnings", "NEFFEX", "/music/neffex/new_beginnings.mp3");
        application->createSongInLibrary("Destiny", "NEFFEX", "/music/neffex/destiny.mp3");
        application->createSongInLibrary("Channa Mereya", "Arijit Singh", "/music/arijit_singh/channa_mereya.mp3");
        application->createSongInLibrary("Kesariya", "Arijit Singh", "/music/arijit_singh/kesariya.mp3");

        // playlist and adding songs to it
        application->createPlaylist("Neffex Songs");
        application->addSongToPlaylist("Neffex Songs", "Cold");
        application->addSongToPlaylist("Neffex Songs", "Destiny");
        application->addSongToPlaylist("Neffex Songs", "New Beginnings");

        application->createPlaylist("Silent Songs");
        application->addSongToPlaylist("Silent Songs", "Channa Mereya");
        application->addSongToPlaylist("Silent Songs", "Kesariya");

        // connect a device
        application->connectAudioDevice(DeviceType::BLUETOOTH);

        // play or pause
        application->playSingleSong("Cold");
        application->pauseCurrentSong("Cold");
        application->playSingleSong("Cold"); // resume

        cout << endl
             << "------- Sequential Playback --------" << endl;
        application->selectPlayStrategy(PlayStrategyType::SEQUENTIAL);
        application->loadPlaylist("Silent Songs");
        application->playAllTracksInPlaylist();

        cout << endl
             << "------- Random Playback --------" << endl;
        application->selectPlayStrategy(PlayStrategyType::RANDOM);
        application->loadPlaylist("Neffex Songs");
        application->playAllTracksInPlaylist();

        cout << endl
             << "------- Custom Playback --------" << endl;
        application->selectPlayStrategy(PlayStrategyType::CUSTOM);
        application->loadPlaylist("Silent Songs");
        application->queueSongNext("Cold");
        application->queueSongNext("Destiny");
        application->playAllTracksInPlaylist();

        cout << "\n-- Play Previous in Sequential --\n";
        application->selectPlayStrategy(PlayStrategyType::SEQUENTIAL);
        application->loadPlaylist("Neffex Songs");
        application->playAllTracksInPlaylist();

        application->playPreviousTrackInPlaylist();
        application->playPreviousTrackInPlaylist();
    }
    catch (const exception& err) {
        cout << "Error: " << err.what() << endl;
    }
    return 0;
}