#ifndef SONG_H
#define SONG_H

#include<string>
class Song
{
public:
    Song();
    Song(std::string title, unsigned int track, std::string album, std::string artist, int duration, std::string genre);
    ~Song();
    std::string getTitle() const;
    void setTitle(const std::string &newTitle);

    unsigned int getTrack() const;
    void setTrack(unsigned int newTrack);

    std::string getAlbum() const;
    void setAlbum(const std::string &newAlbum);

    std::string getArtist() const;
    void setArtist(const std::string &newArtist);

    int getDuration() const;
    void setDuration(int newDuration);

    std::string getGenre() const;
    void setGenre(const std::string &newGenre);

    std::string getPublisher() const;
    void setPublisher(const std::string &newPublisher);

    std::string getFileName() const;
    void setFileName(const std::string &newFileName);

    std::string getFilePath() const;
    void setFilePath(const std::string &newFilePath);

private:
    std::string filePath;
    std::string fileName;
    std::string title;
    unsigned int track;
    std::string album;
    std::string artist;
    int duration;
    std::string genre;
    std::string publisher;
    std::string publish_year;

};

#endif // SONG_H
