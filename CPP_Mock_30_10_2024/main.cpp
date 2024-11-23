#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<iostream>
#include "musicplayer.h"
#include<taglib/fileref.h>
#include<taglib/tag.h>
#include<taglib/audioproperties.h>
#include<string>
#include<fstream>
#include<filesystem>
#include"musicmodel.h"
#include<ncurses.h>
#include"musicmodel.h"
#include"musiccontroller.h"
#include"videocontroller.h"
#include<iostream>
#include"videoplayer.h"
using namespace  std;

extern "C" {
#include <libavformat/avformat.h>
#include<libavcodec/avcodec.h>
}
#include<iomanip>
int main(int argc, char* argv[]){



    while(1){
        std::cout<<"1. Music"<<std::endl;
        std::cout<<"2. Video"<<std::endl;
        std::cout<<"3. Exit"<<std::endl;
        std:: cout<<"Chon option: "<<std::endl;
        std::cin.clear();
        char opt = getchar();
        system("clear");
        if(opt == '1'){
            MusicController* mCtr = MusicController::getInstance();
            mCtr->showMainMenu();
        }
        else if(opt == '2')
        {
            VideoController* vCtr = VideoController::getInstance();
            vCtr->showMainMenu();
        }
        else if(opt == '3') break;
        else {
            std::cout<<"Invalid option. Please try again."<<std::endl;
        }

        std::cout<<"\nPress enter to continue..."<<std::endl;
        getchar();
    }



    return 0;
}




/*+Pagination, 25 per page ==> để sau ==> Tập trung vào chơi nhạc từ danh sách
    How to create Pagination


ncurses
-- Navigate list of
https://www.youtube.com/watch?v=ucmigNoLPeg&list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v&index=9
*/





