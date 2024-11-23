#include "videoplayer.h"
#include<filesystem>
#include<fstream>
#include<iostream>
#include<filesystem>
#include<thread>
#include<chrono>

VideoPlayer::VideoPlayer(): vlcInstance(nullptr), mediaPlayer(nullptr), currentMedia(nullptr), paused(false) {
    initVLC();
    // vPlaylist.push_back("/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/videos/Snaptik.app_7017493814600273154.mp4");
    // vPlaylist.push_back("/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/videos/Snaptik.app_7130182507894492442.mp4");
}

VideoPlayer *VideoPlayer::getInstance()
{
    static VideoPlayer instance;
    return &instance;
}



void VideoPlayer::addToPlayList(const std::string &filePath)
{
    std::lock_guard<std::mutex> lock(mutex);

    playlist.push(filePath);
    vPlaylist.push_back(filePath);

}
void VideoPlayer::addToPlayList(Video * video)
{
    std::lock_guard<std::mutex> lock(mutex);

    vPlayLists.push_back(video);

}

void VideoPlayer::clearPlayList()
{
    std::lock_guard<std::mutex> lock(mutex); // tranh nhieu thread vao ham
    std::queue<std::string> empty;
    std::swap(playlist, empty); // empty se tu dung giai phong khi roi khoi ham
    vPlaylist.clear();
    vPlayLists.clear();
}

bool VideoPlayer::playNext()
{
    std::lock_guard<std::mutex> lock(mutex);
    // if(playlist.empty()){
    //     return false;
    // }
    nextSong = true;
    curFileIndex = (curFileIndex + 1)% vPlaylist.size();
    playPlayList();
    // std::string nextFile = playlist.front(); // lay file o phia truoc
    // playlist.pop(); // pop tu front

    // if(initMediaPlayer(nextFile)){ // 2 viec trong 1 dong code: xu ly va check thanh cong
    //     return play();
    // }
    return false;
}

bool VideoPlayer::playPrevious()
{
    curFileIndex = (curFileIndex -1 + vPlaylist.size() )% vPlaylist.size();
    playPlayList();
}

int VideoPlayer::getPlayListSize()
{
    std::lock_guard<std::mutex> lock(mutex);
    return vPlaylist.size();
}

void VideoPlayer::showPlayList()
{

}
// for auto play
bool VideoPlayer::videoisFinished(){
    while(1){

        if(videoFinished)
        {
            curFileIndex = (curFileIndex + 1)% vPlayLists.size();
            videoFinished = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        if(!vlcInstance) break;
    }
}

void VideoPlayer::playPlayList()
{
    // if(vPlaylist.size()!=0){
    //     curFileIndex = (curFileIndex + 1)% vPlaylist.size();
    //     std::cout<<"curFileIndex "<<curFileIndex;
    //     std::cout<<"is playing: "<<libvlc_media_player_is_playing(mediaPlayer)<<std::endl;
    //     play(vPlaylist[curFileIndex]);
    // }else {
    //     std::cout<<"playlist empty!!"<<std::endl;
    //     return;
    // }

    do{

        play(vPlayLists[curFileIndex]->getFilePath());
        std::this_thread::sleep_for(std::chrono::milliseconds((long)vPlayLists[curFileIndex]->getDuration()*1000));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        videoFinished = true;

        while(videoFinished){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

    }
    while(curFileIndex!=0);
}





// bool VideoPlayer::play()
// {
//     if(!mediaPlayer){ // lam 1 cong viec
//         if(playlist.empty()){
//             return false;
//         }
//         return playNext(); // lam 2 cong viec
//     }
//     if (libvlc_media_player_is_playing(mediaPlayer)){
//         return true;
//     }

//     if (libvlc_media_player_play(mediaPlayer)<0){
//         std::cerr<<"Failed to play media"<<std::endl;
//         return false;
//     }



//     paused = false;
//     while(libvlc_media_player_is_playing(mediaPlayer)){
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }

//     return true;

// }



bool VideoPlayer::play(std::string filePath)
{
     // lam 1 cong viec
    initMediaPlayer(filePath);

    if (libvlc_media_player_is_playing(mediaPlayer)){
        return true;
    }


    if (libvlc_media_player_play(mediaPlayer)<0){
        std::cerr<<"Failed to play media"<<std::endl;
        return false;
    }



    paused = false;



    return true;

}


bool VideoPlayer::pause()
{
    if (!mediaPlayer|| !isPlayLing()){
        return false;
    }

    libvlc_media_player_set_pause(mediaPlayer, 1);
    paused = true;
    return true;

}

bool VideoPlayer::resume()
{
    if (!mediaPlayer|| isPlayLing()){
        return false;
    }
    libvlc_media_player_set_pause(mediaPlayer,0);
    paused = false;
    return true;
}

bool VideoPlayer::stop()
{
    if (!mediaPlayer) return false;
    libvlc_media_player_stop(mediaPlayer);
    paused = false;
    curFileIndex = -1;
    return true;
}


bool VideoPlayer::isPlayLing()
{
    return mediaPlayer&&libvlc_media_player_is_playing(mediaPlayer);
}

bool VideoPlayer::isPause()
{
    return paused;

}

void VideoPlayer::cleanUp()
{
    releaseCurrentMedia();
    if(vlcInstance){
        libvlc_release(vlcInstance);
        vlcInstance = nullptr; // Y tuong giong anh Tuan Anh noi, tranh con tro tro den bo nho rac

    }

}

VideoPlayer::~VideoPlayer()
{

}

bool VideoPlayer::initVLC() // giong vs sdl
{
    const char*args[] = {
        "--quiet",
        "--no-video-title-show",
        "--no-xlib"
    };
    vlcInstance = libvlc_new(3, args); // so luong args va args tu cli

    // check
    if(!vlcInstance){
        std::cerr<<"Failed to create VLC instance. Error: "<<libvlc_errmsg()<<std::endl;
        return false;
    }
    return true;
}

bool VideoPlayer::initMediaPlayer(const std::string &filePath)
{
    if(!validateFile(filePath)){
        return false;
    }



    releaseCurrentMedia();
    std::string absolutePath = std::filesystem::absolute(filePath).string();

    currentMedia = libvlc_media_new_path(vlcInstance, absolutePath.c_str());

    // check
    if(!currentMedia){
        std::cerr<<"Failed to create media: "<<absolutePath<<std::endl;
        return false;
    }

    // Tao mediaplayer
    mediaPlayer = libvlc_media_player_new_from_media(currentMedia);
    //check
    if(!mediaPlayer){
        std::cerr<<"Failed to create a media player"<<std::endl;
        libvlc_media_release(currentMedia);
        currentMedia = nullptr;
        return false;
    }


    // Disable video output

    libvlc_video_set_callbacks(mediaPlayer, nullptr, nullptr, nullptr, nullptr);
    libvlc_video_set_format(mediaPlayer, "none", 0,0,0); // dinh dang va kich thuoc

    currentFile = filePath;
    return true;

}

void VideoPlayer::releaseCurrentMedia()
{
    // neu van con tai nguyen thi giai phong
    if(mediaPlayer){
        libvlc_media_player_stop(mediaPlayer);
        libvlc_media_player_release(mediaPlayer);
    }
    // du lieu
    if(currentMedia){
        libvlc_media_release(currentMedia);
        currentMedia = nullptr;
    }
}

void VideoPlayer::increaseVolume(){
    if(!mediaPlayer) return;
    int curVol = libvlc_audio_get_volume(mediaPlayer);
    int newVol = curVol + 10;

    if(newVol>100) newVol = 100;
    libvlc_audio_set_volume(mediaPlayer, newVol);
}

void VideoPlayer::decreaseVolume(){
    if(!mediaPlayer) return;
    int curVol = libvlc_audio_get_volume(mediaPlayer);
    int newVol = curVol - 10;

    if(newVol<0) newVol = 0;
    libvlc_audio_set_volume(mediaPlayer, newVol);
}


bool VideoPlayer::validateFile(const std::string &filePath)
{

    return true;
}

std::vector<std::string> VideoPlayer::getVPlaylist() const
{
    return vPlaylist;
}

void VideoPlayer::setVPlaylist(const std::vector<std::string> &newVPlaylist)
{
    vPlaylist = newVPlaylist;
}

// code from claude ai:
/*
    1. Khoi tao VLC
    2. Valid file
    3. Tao media
    4. Tao Media Player
    5. Disable video output
    6. Play Media
*/
