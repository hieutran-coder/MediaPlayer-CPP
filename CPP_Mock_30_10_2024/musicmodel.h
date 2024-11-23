#ifndef MUSICMODEL_H
#define MUSICMODEL_H
#include"song.h"
#include <memory>
#include<vector>
#include<map>

class MusicModel
{
public:
    MusicModel();
    static MusicModel *getInstance();
    ~MusicModel();


    std::vector<Song *> getListSongs() ;
    void setListSongs(const std::vector<Song *> &newListSongs);
    int getSize();
    std::string getPath() const;

    std::map<std::string, int> getSongMetaIndex() const;

private:
    std::vector<Song* > listSongs;
    std::string path = "/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/song";

    std::map<std::string, int> songMetaIndex;
};

#endif // MUSICMODEL_H
