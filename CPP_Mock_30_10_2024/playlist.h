#ifndef PLAYLIST_H
#define PLAYLIST_H
#include"song.h"
#include<vector>
#include<filesystem>
class Playlist
{
public:
    Playlist();



    std::string getPlayListName() const;
    void setPlayListName(const std::string &newPlayListName);
    void addSong(Song* song);
    void removeSong(Song* song);


    std::vector<Song *> getListOfSong() ;
    void setListOfSong(const std::vector<Song *> &newListOfSong);

    int getCurrentSongIdx() const;
    void setCurrentSongIdx(int newCurrentSongIdx);

    void play();
private:
    std::string playListName;
    std::vector<Song*> listOfSong;

    int currentSongIdx;
    std::string path = "/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/song";

};

#endif // PLAYLIST_H
