#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H
#include "video.h"
#include<iostream>
#include<vector>
class VideoModel
{
public:
    VideoModel();
    static VideoModel *getInstance();
    ~VideoModel();
    int getSize();

    std::vector<Video *> getListOfVideo() const;
    void setListOfVideo(const std::vector<Video *> &newListOfVideo);

private:
    std::vector<Video*> listOfVideo;


};

#endif // VIDEOMODEL_H
