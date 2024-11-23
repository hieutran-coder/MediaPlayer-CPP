#ifndef MUSICCONTROLLER_H
#define MUSICCONTROLLER_H
#include"musicmodel.h"
#include"musicplayer.h"
#include <memory>
#include<ncurses.h>
#include"playlistcontroller.h"
#include"videomodel.h"
#include"videocontroller.h"
class MusicController
{
public:
    MusicController();
    static MusicController* getInstance();

    MusicModel* musicModel;
    MusicPlayer* musicPlayer;
    PlayListController* playlistController;
    void init();

    ~MusicController();

    void showMainMenu();

    void showMediaFiles();

    void showPlayLists();

    void viewPlayListControl();



    void displaySongMetaData(Song* selectedSong);
    void viewPlayList(Playlist *playlist);
    void showPlayListMenu();
    void viewPlayList();
    void showPlayBackControl(Song *selectedSong);
    void editSongMetaData(Song *selectedSong);
    void setMetaDataOfSong(int i, Song* selectedSong);

    void showPlayBackControlForPlayList(Song *selectedSong);
private:
    void playPlayList(Playlist *playlist);
    VideoModel *videoModel;
    VideoController* videoController;
};

#endif // MUSICCONTROLLER_H
