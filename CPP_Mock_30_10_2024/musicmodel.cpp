#include "musicmodel.h"
#include<taglib/fileref.h>
#include<taglib/tag.h>
#include<taglib/audioproperties.h>
#include<filesystem>
#include<map>
MusicModel::MusicModel() {

    songMetaIndex["title"] = 0;
    songMetaIndex["artist"] = 1;
    songMetaIndex["album"] = 2;
    songMetaIndex["genre"] = 3;

    std::string title, album, artist, genre;
    unsigned int     track;
    int duration;
    std::filesystem::path relativePath("song");
    std::filesystem::path absolutePath = std::filesystem::path(relativePath);
    std::cout << "Absolute File Path: " << absolutePath << std::endl;

    for (const auto & entry : std::filesystem::recursive_directory_iterator(absolutePath.string())) {
        if(std::filesystem::is_regular_file(entry)){

            std::cout<< entry.path().filename().string()<<std::endl;



            TagLib::FileRef file( entry.path().c_str() );

            if (!file.isNull()&& file.tag()){
                TagLib::Tag *tag = file.tag();
                std::cout<<"-----------------******--------------"<<std::endl;
               // std::cout<<"Title: "<<tag->title().to8Bit()<<std::endl;
                title = tag->title().to8Bit();
               // std::cout<<"Artist: "<<tag->artist().to8Bit()<<std::endl;
                artist = tag->artist().to8Bit();
               // std::cout<<"Album: "<<tag->album().to8Bit()<<std::endl;
                album = tag->album().to8Bit();
               // std::cout<<"Track: "<<tag->track()<<std::endl;
                track = tag->track();
                //std::cout<<"Genre: "<<tag->genre().to8Bit()<<std::endl;
                genre = tag->genre().to8Bit();
              //  std::cout<<"Publisher: "<<std::endl;
            }

            int sLength, minutes, seconds;
            // Lấy duration
            if (!file.isNull()&& file.audioProperties()){
                TagLib::AudioProperties *properties = file.audioProperties();

                sLength = properties->length();
                minutes = sLength/60;
                seconds = sLength%60;
            } else{
                std::cerr<<" Error: Can not retrieve audio properties"<<std::endl;
            }
            std::cout<<"Duration: "<<sLength<<std::endl;
            duration = sLength;
            std::cout<<"-----------------******--------------"<<std::endl;

            Song *song = new Song(title, track, album, artist, duration, genre);
            song->setFileName(entry.path().filename().string());
            song->setFilePath(entry.path());
            std::cout<<"Song: "<<song->getArtist()<<std::endl;

            listSongs.push_back(song);
        }


    }

    std::cout<<"size: "<<(int)listSongs.size();

}

MusicModel *MusicModel::getInstance()
{
    static MusicModel instance;
    return &instance;
}

MusicModel::~MusicModel()
{
    for(int i = 0; i< listSongs.size(); i++){
        std::cout<<"Xoa Song "<<i<<std::endl;
        delete listSongs[i];

    }
}

std::vector<Song *> MusicModel::getListSongs()
{
     return listSongs;
}



void MusicModel::setListSongs(const std::vector<Song *> &newListSongs)
{
    listSongs = newListSongs;
}

int MusicModel::getSize()
{
    return listSongs.size();
}

std::string MusicModel::getPath() const
{
    return path;
}

std::map<std::string, int> MusicModel::getSongMetaIndex() const
{
    return songMetaIndex;
}


