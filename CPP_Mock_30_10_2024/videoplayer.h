#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H
#include "video.h"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>
#include <vlc/vlc.h>
#include<thread>
#include<queue>
#include<vector>
// File nay prompt to claude AI va go lai
class VideoPlayer
{
public:
    VideoPlayer();
    static VideoPlayer* getInstance();




    // Quan ly playlist
    void addToPlayList(const std::string& filePath);
    void clearPlayList();
    bool playNext();
    bool playPrevious();
    int getPlayListSize();
    void showPlayList();
    void playPlayList();

    // PLayBack controll
    //bool play();
    bool play(std::string filePath);

    bool pause();
    bool resume();
    bool stop();
    bool isPlayLing();
    bool isPause();


    //Quan ly tai nguyen
    void cleanUp();
    ~VideoPlayer();
    std::vector<std::string> getVPlaylist() const;
    void setVPlaylist(const std::vector<std::string> &newVPlaylist);

    void addToPlayList(Video *video);
    void increaseVolume();
    void decreaseVolume();

    bool videoisFinished();
private:
    bool videoFinished = false;
    bool initVLC();
    bool initMediaPlayer(const std::string& filePath);

    // quan ly trang thai ben trong
    void releaseCurrentMedia();
    bool validateFile(const std::string& filePath);

    //vlc instance vs mediaplayer
    libvlc_instance_t* vlcInstance;
    libvlc_media_player_t* mediaPlayer;
    libvlc_media_t* currentMedia;

    // Quan ly playlist
    std::queue<std::string> playlist;
    std::vector<std::string> vPlaylist;
    std::vector<Video*> vPlayLists;
    std::string currentFile;
    int curFileIndex = 0;
    bool paused;
    std::condition_variable cv;
    bool nextSong = false;
    // thread
    std::mutex mutex;


};

#endif // VIDEOPLAYER_H
