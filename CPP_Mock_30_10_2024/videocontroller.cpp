#include "videocontroller.h"
#include "videoplayer.h"
#include<iostream>
using namespace  std;
VideoController::VideoController() {

    videoModel = VideoModel::getInstance();
    videoPlayer = VideoPlayer::getInstance();
}

VideoController *VideoController::getInstance()
{
    static VideoController instance;
    return &instance;
}

void VideoController::showMediaFiles()
{
    system("clear");

    // Lam sao tranh lap lai lan 2
    while(1){
        for(int i = 0; i<videoModel->getSize(); i++){
            std::cout<<i+1<<"."<<videoModel->getListOfVideo()[i]->getFileName()<<std::endl;
        }
        std::cout<<videoModel->getSize() + 1<<". Exist"<<std::endl;

        std::cin.clear();
        int input;
        //system("clear");
        std::cout<<"Chon option: "<<std::endl;
        std::cin>>input;

        for(int i = 1; i<videoModel->getSize(); i++){
            if(input == i){
                showVideoMetaData(videoModel->getListOfVideo()[i-1]);
                break;
            }

        }
        if (input == videoModel->getSize() + 1) break;

        // std::cout<<"\nPress enter to continue..."<<std::endl;
        // getchar();
    }


}

void VideoController::showPlayBackControl(Video* video){
    system("clear");
    while(1){
        std::cout<<"1. Name: "<<video->getFileName()<<std::endl;
        std::cout<<"2. Duration: "<<video->getDuration()<<std::endl;
        std::cout<<"3. Size: "<<video->getSize()<<std::endl;
        std::cout<<"4. Codec: "<<video->getCodec()<<std::endl;
        std::cout<<"5. BitRate: "<<video->getBitRate()<<std::endl;

        std::cout<<"6. Play"<<std::endl;
        std::cout<<"7. Pause"<<std::endl;
        std::cout<<"8. Resume"<<std::endl;
        std::cout<<"9. Stop"<<std::endl;
        std::cout<<"10. Increse volume"<<std::endl;
        std::cout<<"11. Decrease volume"<<std::endl;
        std::cout<<"12. Add to PlayList "<<std::endl;
        std::cout<<"13. Exist "<<std::endl;


        std::cin.clear();
        int opt ;
        std::cout<<"Chon option: "<<std::endl;
        std::cin>>opt;
        if(opt == 6){
            // playing
            videoPlayer->play(video->getFilePath());
          // videoPlayer->play();
        }
        else if(opt == 7)
        {
            videoPlayer->pause();
        }
        else if(opt == 8){
            videoPlayer->resume();
        }
        else if(opt == 9){
            videoPlayer->stop();
        }
        else if(opt == 10){
            videoPlayer->increaseVolume();
        }
        else if(opt == 11){
            videoPlayer->decreaseVolume();

        }
        else if(opt == 12){
            videoPlayer->addToPlayList(video->getFilePath());
            videoPlayer->addToPlayList(video);
        }
        else if(opt == 13){
            break;
        }
        else {
            // std::cout<<"Invalid option. Please try again."<<std::endl;
        }

        // std::cout<<"\nPress enter to continue..."<<std::endl;
        // getchar();
    }

}

void VideoController::showVideoMetaData(Video *video)
{
    system("clear");
    while(1){
        std::cout<<"=======METADATA========: "<<std::endl;

        std::cout<<"1. Name: "<<video->getFileName()<<std::endl;
        std::cout<<"2. Duration: "<<video->getDuration()<<std::endl;
        std::cout<<"3. Size: "<<video->getSize()<<std::endl;
        std::cout<<"4. Codec: "<<video->getCodec()<<std::endl;
        std::cout<<"5. BitRate: "<<video->getBitRate()<<std::endl;
        std::cout<<"===================: "<<std::endl;

        std::cout<<"6. Play"<<std::endl;
        std::cout<<"7. Add to PlayList "<<std::endl;
        std::cout<<"8. Exist "<<std::endl;


        std::cin.clear();
        char opt = getchar();
        std::cout<<"Chon option: "<<std::endl;

        if(opt == '6'){
            // playing
            cout<<"filePath: "<<video->getFilePath();
            //videoPlayer->addToPlayList(video->getFilePath());
            std::thread th1([this, &video](){videoPlayer->play(video->getFilePath());});
            th1.detach();
            showPlayBackControl(video);
        }
        else if(opt == '7')
        {
            videoPlayer->addToPlayList(video->getFilePath());
            videoPlayer->addToPlayList(video);
        }
        else if(opt == '8') break;
        else {
           // std::cout<<"Invalid option. Please try again."<<std::endl;
        }

        // std::cout<<"\nPress enter to continue..."<<std::endl;
        // getchar();
    }
}

void VideoController::showPlayList()
{
    while(1){
        for(int i = 0; i<videoPlayer->getPlayListSize(); i++){
            cout<<i+1<<". "<<videoPlayer->getVPlaylist()[i]<<endl;
        }
        cout<<"================================================\n";
        cout<<"1. Enter 1 to play \n";
        cout<<"2. Enter 2 to clear this play this\n";
        cout<<"3. Enter 3 to return  \n";
        std::cin.clear();
        char opt = getchar();
        if(opt == '1'){
            showPlayBackControlForPlayList();
        }
        else if(opt == '3')
        {
            break;
        }
        else if(opt == '2')
        {
            videoPlayer->clearPlayList();
        }

     }
}

void VideoController::showPlayBackControlForPlayList()
{
    system("clear");
    while(1){
        // std::cout<<"1. Name: "<<video->getFileName()<<std::endl;
        // std::cout<<"2. Duration: "<<video->getDuration()<<std::endl;
        // std::cout<<"3. Size: "<<video->getSize()<<std::endl;
        // std::cout<<"4. Codec: "<<video->getCodec()<<std::endl;
        // std::cout<<"5. BitRate: "<<video->getBitRate()<<std::endl;
        std::cout<<"==========PLAYING PLAYLIST==========="<<std::endl;
        std::cout<<"6. Play"<<std::endl;
        std::cout<<"7. Pause"<<std::endl;
        std::cout<<"8. Resume"<<std::endl;
        std::cout<<"9. Stop"<<std::endl;
        std::cout<<"10. Increse volume"<<std::endl;
        std::cout<<"11. Decrease volume"<<std::endl;
        std::cout<<"12. Next Video "<<std::endl;
        std::cout<<"13. Previous Video "<<std::endl;
        std::cout<<"14. Exist "<<std::endl;


        std::cin.clear();
        int opt ;
        std::cout<<"Chon option: "<<std::endl;
        std::cin>>opt;
        if(opt == 6){
            // playing
            std::thread th2([this](){videoPlayer->playPlayList();});
            th2.detach();
            std::thread th3([this](){videoPlayer->videoisFinished();});
            th3.detach();
            // videoPlayer->play();
        }
        else if(opt == 7)
        {
            videoPlayer->pause();
        }
        else if(opt == 8){
            videoPlayer->resume();
        }
        else if(opt == 9){
            videoPlayer->stop();
        }
        else if(opt == 10){
            videoPlayer->increaseVolume();
        }
        else if(opt == 11){
            videoPlayer->decreaseVolume();
        }
        else if(opt == 12){
            std::thread th4([this](){videoPlayer->playNext();});
            th4.detach();
        }
        else if(opt == 13){
            std::thread th5([this](){videoPlayer->playPrevious();});
            th5.detach();
        }

        else if(opt == 14){
            break;
        }
        else {
            // std::cout<<"Invalid option. Please try again."<<std::endl;
        }


    }

}

void VideoController::showMainMenu()
{

    while(1){

        cout<<"1. Video files"<<endl;
        cout<<"2. Playlist"<<endl;
        cout<<"3. Exist"<<endl;

        std::cin.clear();
        char opt = getchar();
        std::cout<<"Chon option: "<<std::endl;

       // system("clear");
        if(opt == '1'){
            showMediaFiles();
        }
        else if(opt == '2')

        {
            showPlayList();
        }
        else if(opt == '3') break;
        else {
           // std::cout<<"Invalid option. Please try again."<<std::endl;
        }



    }

}
