#ifndef MusicPlayer_H
#define MusicPlayer_H
#include"musicmodel.h"
#include <condition_variable>
#include<vector>
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<mutex>
#include<thread>
class MusicPlayer
{
public:
    MusicPlayer();

    static MusicPlayer *getInstance();
    void initSDL();
    void cleanUpSDL();
    void playMusic(const char *filePath);


    int loadMusic(std::string fileName);

    void increaseVolume();
    void decreaseVolume();

    static void onMusicFinished();

    std::vector<Mix_Chunk*> sounds;
    std::vector<Mix_Music*> lMusic;
    void setVolume(int v);
    int playMusic(int m);
    void quitMixer();
    void togglePlay();
    void stopMusic();
    int getCurrentPos() ;
    void setCurrentPos(int newCurrentPos);

    int getVolume() const;

    int playMusicWithPlayList(std::vector<Song *> listOfSong);
    Song *nextSong();
    int playCurrentSong();
    void cleanUpMusic();

    void checkMusicFinished();
    int getCurrentSongIndex() const;

    void previousSong();
private:
    int volume = 80;
    std::mutex mutex;
    bool shouldToggle = false;
    std::condition_variable cv;
    bool shouldStop = false;
    int currentPos = 0;
    bool volumeChanged = false;
    Mix_Music* music = NULL;

    int currentSongIndex = 0;
    bool musicFinished = false;
    int curentSize;
    int sizeOfListSong;
    std::vector<Song*> listOfSongs;
};

#endif // MusicPlayer_H
