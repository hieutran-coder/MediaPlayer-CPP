#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H
#include"videomodel.h"
#include "videoplayer.h"
class VideoController
{
public:
    VideoController();
    static VideoController* getInstance();
    void showMainMenu();
    void showMediaFiles();
    void showPlayBackControl(Video *video);
private:
    VideoModel* videoModel;
    void showVideoMetaData(Video * video);
    VideoPlayer *videoPlayer;

    void showPlayList();
    void showPlayBackControlForPlayList();
};

#endif // VIDEOCONTROLLER_H
