#include "song.h"
#include<iostream>
Song::Song() {}

Song::Song(std::string title,
           unsigned int track,
           std::string album,
           std::string artist,
           int duration,
           std::string genre): title(title), track(track), album(album), artist(artist), duration(duration), genre(genre)
{

}

Song::~Song()
{

}

std::string Song::getTitle() const
{
    return title;
}

void Song::setTitle(const std::string &newTitle)
{
    title = newTitle;
}

unsigned int Song::getTrack() const
{
    return track;
}

void Song::setTrack(unsigned int newTrack)
{
    track = newTrack;
}

std::string Song::getAlbum() const
{
    return album;
}

void Song::setAlbum(const std::string &newAlbum)
{
    album = newAlbum;
}

std::string Song::getArtist() const
{
    return artist;
}

void Song::setArtist(const std::string &newArtist)
{
    artist = newArtist;
}

int Song::getDuration() const
{
    return duration;
}

void Song::setDuration(int newDuration)
{
    duration = newDuration;
}

std::string Song::getGenre() const
{
    return genre;
}

void Song::setGenre(const std::string &newGenre)
{
    genre = newGenre;
}

std::string Song::getPublisher() const
{
    return publisher;
}

void Song::setPublisher(const std::string &newPublisher)
{
    publisher = newPublisher;
}

std::string Song::getFileName() const
{
    return fileName;
}

void Song::setFileName(const std::string &newFileName)
{
    fileName = newFileName;
}

std::string Song::getFilePath() const
{
    return filePath;
}

void Song::setFilePath(const std::string &newFilePath)
{
    filePath = newFilePath;
}
