#include "view.h"
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

extern "C" {
#include <libavformat/avformat.h>
#include<libavcodec/avcodec.h>
}
#include<iomanip>

View::View() {}


// Basic menu


long getFileSize(const std::string& fileName){
    std::ifstream file(fileName, std::ios::binary|std::ios::ate);
    return file.tellg();

}
int showVideoMetadata(){
    // VIDEO Metadata
    avformat_network_init();
    AVFormatContext* formatContext = nullptr;
    std::string fileName = "/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/videos/GioLayNhanhDao-TuNamTiNaTichAmXa-7026106.mp4";

    // Open file input
    if(avformat_open_input(&formatContext, fileName.c_str(), nullptr, nullptr) != 0){
        std::cerr << "Could not open file: " << fileName << std::endl;
        return -1;
    }

    // Retrieve stream info
    if(avformat_find_stream_info(formatContext, nullptr)<0){
        std::cerr << "Could not open file stream info: " << fileName << std::endl;
        avformat_close_input(&formatContext);
        return -1;
    }

    std::cout<<"Video Name: "<<fileName<<std::endl;
    std::cout<<"Video size: "<<getFileSize(fileName)<<" bytes"<<std::endl;
    std::cout<<"Duration: "<<static_cast<double>(formatContext->duration)/AV_TIME_BASE<<" seconds"<<std::endl;

    for(unsigned int i = 0; i < formatContext->nb_streams; ++i){
        AVStream* stream = formatContext->streams[i];
        AVCodecParameters* codecParams = stream->codecpar;


        // Kiểm tra nếu stream là video stream
        if(codecParams->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            std::cout<<"BitRate: "<<codecParams->bit_rate<<" bít per seconds"<<std::endl;
            std::cout<<"Codec: "<<avcodec_get_name(codecParams->codec_id)<<std::endl;
            break;
        }
    }

    // Clean
    avformat_close_input(&formatContext);
    avformat_network_deinit();

    return 0;
}

// Cần edit metadata cho audio và video nữa
void showMusicMetaData(std::string fileName){

    TagLib::FileRef file(fileName.c_str());

    if (!file.isNull()&& file.tag()){
        TagLib::Tag *tag = file.tag();

        std::cout<<"Title: "<<tag->title()<<std::endl;
        std::cout<<"Artist: "<<tag->artist()<<std::endl;
        std::cout<<"Album: "<<tag->album()<<std::endl;
        std::cout<<"Track: "<<tag->track()<<std::endl;
        std::cout<<"Genre: "<<tag->genre()<<std::endl;
        std::cout<<"Publisher: ";
    }

    int sLength, minutes, seconds;

    if (!file.isNull()&& file.audioProperties()){
        TagLib::AudioProperties *properties = file.audioProperties();

        sLength = properties->length();
        minutes = sLength/60;
        seconds = sLength%60;
    } else{

        std::cerr<<" Error: Can not retrieve audio properties"<<std::endl;
    }

    std::cout<<"Duration: "<<sLength<<std::endl;

}

void showMediaFiles(){
    // Show audio and video files
    // article 1: https://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c
    std::string path = "/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/song";

    for (const auto & entry : std::filesystem::recursive_directory_iterator(path)) {
        if(std::filesystem::is_regular_file(entry)){
            std::cout<< entry.path().filename().string()<<std::endl;
        }
    }
}


void playWithNCurses4(){
    initscr();
    noecho();
    cbreak();

    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* menuwin = newwin(6,xMax-12,yMax-5,5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);


    keypad(menuwin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...

    std::string choices[5] = {"Walk", "Job", "Run"};
    int choice;
    int highlight = 0;

    // su dung vong lap while de hien thi menu
    while(1){
        for(int i = 0; i<3; i++){
            if(i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i+1, 1, choices[i].c_str());
            wattroff(menuwin,A_REVERSE);
        }
        choice = wgetch(menuwin);

        switch(choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight == 3)
                highlight = 2;
            break;
        default:
            break;
        }
        if (choice == 10)
            break;

    }

    printw("Your choice was: %s", choices[highlight].c_str());

    getch();
    endwin();
}



// user input
void playWithNCurses3(){
    initscr();
    noecho();
    cbreak();

    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* inputwin = newwin(3,xMax-12,yMax-5,5);
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);



    // press j
    // if(c == 'j'){
    //     mvwprintw(inputwin, 1, 1, "You press j");
    //     wrefresh(inputwin);
    // }
    // press up
    keypad(inputwin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...
    int c = wgetch(inputwin);
    if (c== KEY_UP){
        mvwprintw(inputwin, 1,1, "You press up");
        wrefresh(inputwin);
    }


    getch();
    endwin();



}


// Getting info about terminal
void playWithNCurses2(){
    initscr();
    noecho();
    cbreak();

    int y, x, yBeg, xBeg, yMax, xMax;

    WINDOW* win = newwin(10,20,10,10);

    getyx(stdscr, y,x); // Lay toa do hien tai cua con tro
    getbegyx(win, yBeg, xBeg); // lay toa do goc ben trai tren cung (goc bat dau
    getmaxyx(stdscr, yMax, xMax); // Lay kich thuoc

    mvprintw(yMax/2, xMax/2, "%d %d", yBeg, xBeg);

    getch();
    endwin();



}


void playWithNCurses(){
    // //set up memory and clear screen
    // initscr();// Khoi tao screen

    // int x,y;
    // x = y = 10;

    // // move the cursor to specific location
    // move(y,x);
    // printw("Hello world!"); // so sanh vs printf

    // //refresh screen
    // refresh();

    // int c = getch();
    // move(0,0);
    // mvprintw(0,0,"%d", c); // condense two fuction: First: move, second: print



    // getch();

    // //clear screen
    // clear();
    // endwin();

    //////////////////////////////////
    initscr();
    //// three evironment options
    // cbreak();
    raw(); //accept control character like ctr as input
    // noecho();  // what ever input type use input doesn't show up on screen
    int height, width, start_y, start_x;

    height =20; width = 10;
    start_x = start_y = 5;


    WINDOW* win = newwin(height, width, start_y, start_x);
    refresh();
    //box(win, 103, 104);  // tao duong vien voi cac ki tu
    char c = 'g';
    box(win, (int)c, 104); // tao duong vien voi ki tu c (neu ko muon lookup)

    int left, right, top, bottom, tlc, trc, blc, brc;
    wborder(win, left, right, top, bottom, tlc, trc, blc, brc);


    mvwprintw(win,1,1, "this is my box");
    wrefresh(win); // refresh specific window


    attron(A_STANDOUT); // boi trang dong chu
    printw("This is some text");
    attroff(A_STANDOUT);


    c = getch();






    endwin();




}
