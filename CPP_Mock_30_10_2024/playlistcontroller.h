#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H
#include"playlist.h"
#include<vector>
class PlayListController
{
public:
    PlayListController();
    static PlayListController* getInstance();
    void createPlayList();

    void showListOfPlayList();
    void addToPlayList(Song* song);
    void updatePlayList();
    void updatePlayList(Playlist* playlist);

    std::vector<Playlist *> getListOfPlaylist() const;
    void setListOfPlaylist(const std::vector<Playlist *> &newListOfPlaylist);

    void chooseSongFromMediaToAdd(Playlist *playlist);
    void chooseSongToRemove(Playlist *playlist);
    void renameOfPlayList(Playlist* playlist);
    void deletePlayList();

private:
    std::vector<Playlist*> listOfPlaylist;




};

#endif // PLAYLISTCONTROLLER_H
