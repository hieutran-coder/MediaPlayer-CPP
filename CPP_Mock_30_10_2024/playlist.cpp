#include "playlist.h"
#include "ncurses.h"
#include <iostream>
#include <ostream>
#include "musicplayer.h"
#include<thread>

Playlist::Playlist() {

    std::filesystem::path relativePath("song");
    std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
    std::cout << "    Absolute Folder Path: " << absolutePath << std::endl;
    path = absolutePath.string();

}

std::string Playlist::getPlayListName() const
{
    return playListName;
}

void Playlist::setPlayListName(const std::string &newPlayListName)
{
    playListName = newPlayListName;
}



void Playlist::play(){



}


int Playlist::getCurrentSongIdx() const
{
    return currentSongIdx;
}

void Playlist::setCurrentSongIdx(int newCurrentSongIdx)
{
    currentSongIdx = newCurrentSongIdx;
}

void Playlist::addSong(Song *song)
{

    for(int i = 0; i<listOfSong.size(); i++){
        if(listOfSong[i]->getTitle() == song->getTitle()) return;
    }
    listOfSong.push_back(song);
}

void Playlist::removeSong(Song *song)
{
    for(int i = 0; i<listOfSong.size(); i++){
        if(song->getTitle() == listOfSong[i]->getTitle()){
            listOfSong.erase(listOfSong.begin() + i);
            break;
        }

    }
}

std::vector<Song *> Playlist::getListOfSong()
{

    // for(int i = 0; i<listOfSong.size(); i++){
    //     std::cout<<listOfSong[i]->getFileName()<<std::endl;

    // }
    return listOfSong;
}

void Playlist::setListOfSong(const std::vector<Song *> &newListOfSong)
{
    listOfSong = newListOfSong;
}


