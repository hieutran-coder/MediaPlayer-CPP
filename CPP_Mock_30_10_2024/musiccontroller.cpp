#include "musiccontroller.h"
#include <codecvt>
#include <iostream>
#include<locale.h>
#include <locale>
#include <fcntl.h>
#include <memory>
#include<thread>
#include<map>
#include<taglib/fileref.h>
#include"videomodel.h"


MusicController::MusicController() {
    musicModel = MusicModel::getInstance();
    musicPlayer = MusicPlayer::getInstance();
    playlistController = PlayListController::getInstance();
    videoModel = VideoModel::getInstance();
    videoController = VideoController::getInstance();
}

MusicController *MusicController::getInstance()
{
    static MusicController instance;
    return &instance;
}

void MusicController::init()
{
    musicPlayer->initSDL();
    musicPlayer->playMusic("/home/hieutran/Documents/QT_Framework/QT_Advance/CPP_Mock_30_10_2024/song/213-HuaKimTuyenNegav-8412418.mp3");
    musicPlayer->cleanUpSDL();
}

MusicController::~MusicController()
{
    //delete musicModel;
    // delete musicPlayer;
}

void MusicController::showMainMenu()
{

    initscr();
    noecho();
    cbreak();
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW* mainMenuWin = newwin(15, xMax -12, 5,5);
    box(mainMenuWin, 0, 0);
    refresh();
    wrefresh(mainMenuWin);
    keypad(mainMenuWin, true);

    mvwprintw(mainMenuWin, 1,1, "=== MAIN MENU ===");
    std::string options[] = {"1. Music \n", "2. Music PlayList \n","3. Video \n", "4. Video Playlist \n", "5. Exist\n"};
    int choice;
    int highlight = 0;
    while(1){

        for (int i = 0; i<5; i++){
            if (i == highlight)
                wattron(mainMenuWin, A_REVERSE);
            mvwprintw(mainMenuWin,i+2, 1, "%s", options[i].c_str());
            wattroff(mainMenuWin, A_REVERSE);
        }

        wrefresh(mainMenuWin);
        choice= wgetch(mainMenuWin);
        if(choice =='\n'&& highlight == 0)
        {
            werase(mainMenuWin);
            wrefresh(mainMenuWin);
            showMediaFiles();
            box(mainMenuWin, 0,0);

        }
        if(choice =='\n'&& highlight == 4)
        {
            break;
        }
        if (choice == '\n' && highlight ==1)
        {
            werase(mainMenuWin);
            wrefresh(mainMenuWin);
            showPlayListMenu();
            box(mainMenuWin, 0,0);
        }

        switch(choice){
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight == 5)
                highlight = 4;
            break;


        default:
            break;

        }
    }
    //getch();
    endwin();
}

// ChatGPT help integrate ncurse with pagenation

void MusicController::showMediaFiles() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int pageSize = 25;  // Number of items to display per page
    int number_of_songs = musicModel->getSize();
    int totalPages = (number_of_songs + pageSize - 1) / pageSize;
    int currentPage = 0;

    WINDOW* menuwin = newwin(pageSize + 3, xMax - 12, 5, 5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin, true);

    int choice;
    int highlight = 0;

    while (true) {
        werase(menuwin);
        box(menuwin, 0, 0);

        // Display items on the current page
        int startIdx = currentPage * pageSize;
        int endIdx = std::min(startIdx + pageSize, number_of_songs);

        for (int i = startIdx; i < endIdx; i++) {
            int displayIdx = i - startIdx;  // Position within the page
            if (i == highlight)
                wattron(menuwin, A_REVERSE);


            mvwprintw(menuwin, displayIdx + 1, 1, "%s", musicModel->getListSongs()[i]->getTitle().c_str());
            wattroff(menuwin, A_REVERSE);
        }

        // Navigation message
        mvwprintw(menuwin, pageSize + 1, 1, "Page %d of %d | Use arrows to navigate, ENTER to select, BACKSPACE to quit", currentPage + 1, totalPages);

        choice = wgetch(menuwin);
        if (choice == KEY_BACKSPACE) {  // Quit
            werase(menuwin);
            wrefresh(menuwin);
            return;
            //break;
        } else if (choice == KEY_DOWN) {
            highlight++;
            if (highlight >= endIdx)
                highlight = startIdx;
        } else if (choice == KEY_UP) {
            highlight--;
            if (highlight < startIdx)
                highlight = endIdx - 1;
        } else if (choice == KEY_RIGHT && currentPage < totalPages - 1) {  // Next page
            currentPage++;
            highlight = currentPage * pageSize;
        } else if (choice == KEY_LEFT && currentPage > 0) {  // Previous page
            currentPage--;
            highlight = currentPage * pageSize;
        } else if (choice == '\n' && highlight < number_of_songs) {  // Select song
            displaySongMetaData(musicModel->getListSongs()[highlight]);
        }

        wrefresh(menuwin);
    }

    delwin(menuwin);
    endwin();
}
// void MusicController::showMediaFiles(){

//     int y, x, yBeg, xBeg, yMax, xMax;
//     getmaxyx(stdscr, yMax, xMax);

//     int number_of_songs = (int)musicModel->getSize();
//     std::cout<<"number of song: "<<number_of_songs;
//     WINDOW* menuwin = newwin(number_of_songs + 3,xMax-12,5,5);
//     box(menuwin, 0, 0);
//     refresh();
//     wrefresh(menuwin);
//     keypad(menuwin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...

//     //std::string choices[5] = {"Walk", "Job", "Run"};

//     int choice;
//     int highlight = 0;

//     // su dung vong lap while de hien thi menu
//     while(1){
//         for(int i = 0; i<number_of_songs; i++){
//             if(i == highlight)
//                 wattron(menuwin, A_REVERSE);
//             // Muc dich: de fix loi phong chu khi hien thi tieng viet ==> Nhung chua xu ly dc
//             std::wstring wide_title;
//             try {
//                 std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//                 wide_title = converter.from_bytes(musicModel->getListSongs()[i]->getTitle());
//             } catch (const std::range_error& e) {
//                 // If the conversion fails, use a placeholder string
//                 wide_title = L"[Unsupported character]";
//             }
//             mvwprintw(menuwin, i+1, 1,"%s", musicModel->getListSongs()[i]->getTitle().c_str());
//            // mvwaddwstr(menuwin, i+1, 1, wide_title.c_str());

//             wattroff(menuwin,A_REVERSE);

//             if (highlight == number_of_songs){ // la
//                 wattron(menuwin, A_REVERSE);
//             }
//             if (i == number_of_songs -1)
//                 mvwprintw(menuwin, i+2, 1,"Final. Return to Main Menu");


//             wattroff(menuwin,A_REVERSE);


//         }

//         choice = wgetch(menuwin);
//         if (choice == '\n'&& highlight == number_of_songs){
//             werase(menuwin);
//             wrefresh(menuwin);
//             delwin(menuwin);
//             return;
//         }

//         switch(choice)
//         {
//         case KEY_UP:
//             highlight--;
//             if (highlight == -1)
//                 highlight = 0;
//             break;
//         case KEY_DOWN:
//             highlight++;
//             if(highlight == number_of_songs+1)
//                 highlight = number_of_songs;
//             break;
//         case 10: // enter
//             if (highlight != number_of_songs){
//                 werase(menuwin);
//                 wrefresh(menuwin);
//                 displaySongMetaData(musicModel->getListSongs()[highlight]);
//                 box(menuwin, 0,0); // Tao vien
//             }
//             break;
//         default:
//             break;
//         }
//         wrefresh(menuwin);
//     }


//     //printw("Your choice was: %s", choices[highlight].c_str());

//     getch();
//     endwin();

// }



void MusicController::displaySongMetaData(Song* selectedSong) // giong nhu ham display showPlayListMenu
{
    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax);


    WINDOW* songMetaDataWin = newwin(20, xMax -12, 5,5);

    box(songMetaDataWin, 0, 0);

    // Hien thi thong tin metadata
    mvwprintw(songMetaDataWin, 1,1, "==SONG METADATA==");
    mvwprintw(songMetaDataWin, 2,1, "Title: %s", selectedSong->getTitle().c_str());
    mvwprintw(songMetaDataWin, 3,1, "Artist: %s", selectedSong->getArtist().c_str());
    mvwprintw(songMetaDataWin, 4,1, "Album: %s", selectedSong->getAlbum().c_str());
    mvwprintw(songMetaDataWin, 6,1, "==SONG METADATA==");

    int duration = selectedSong->getDuration();
    int minutes = duration/60;
    int seconds = duration%60;
    mvwprintw(songMetaDataWin, 5,1,"Duration: %d:%02d", minutes, seconds);




    //Display menu option
    std::string options[] = {"Play", "Edit Metadata", "Add to PlayList", "Return to MediaFiles"};

    int highlight = 0;
    int choice;
    keypad(songMetaDataWin, true);

    while (true) {
        for (int i = 0; i<4; ++i){
            if (i == highlight){
                wattron(songMetaDataWin, A_REVERSE);
            }
            mvwprintw(songMetaDataWin, 8+i,1, "%s", options[i].c_str() );
            wattroff(songMetaDataWin, A_REVERSE);
        }

        choice = wgetch(songMetaDataWin);
        if (choice =='\n' && highlight == 3) {
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            delwin(songMetaDataWin);
            return;
        };

        if (choice == '\n' && highlight == 2){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            playlistController->addToPlayList(selectedSong);

            box(songMetaDataWin, 0,0); // Tao vien
            mvwprintw(songMetaDataWin, 12, 1,"Add Song Successfully");


        }

        if (choice == '\n' && highlight == 1){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            editSongMetaData(selectedSong);
            box(songMetaDataWin, 0,0); // Tao vien
        }

        if (choice == '\n' && highlight == 0){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            showPlayBackControl(selectedSong);
            box(songMetaDataWin, 0,0); // Tao vien
        }
        //Handle navigation and selection
        switch(choice){
        case KEY_UP:
            highlight = (highlight ==0)? 3: highlight -1;
            break;
        case KEY_DOWN:
            highlight = (highlight == 3)? 0: highlight +1;
            break;
        case 10:
            if(highlight == 0){
                // Play song
               // showPlayBackControl(selectedSong);
            }
            else if (highlight == 1){
                // edit meta data
            }
            else if (highlight == 2){
                // add to playlist


            }
            else if (highlight == 3){
                // return to mediafiles
            }
            break;
        default:
            break;
        }
        wrefresh(songMetaDataWin);
    }





}


void MusicController::viewPlayList(Playlist* playlist) // giong nhu ham view MediaFiles
{
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_songs = (int)playlist->getListOfSong().size();
    WINDOW* playListWin = newwin(number_of_songs + 20,xMax-12,5,5);
    box(playListWin, 0, 0);
    refresh();
    wrefresh(playListWin);
    keypad(playListWin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...

    //std::string choices[5] = {"Walk", "Job", "Run"};

    int choice;
    int highlight = 0;

    // su dung vong lap while de hien thi menu
    while(1){
        for(int i = 0; i<=number_of_songs; i++){
            if(i == highlight)
                wattron(playListWin, A_REVERSE);
            if (i<number_of_songs){
                mvwprintw(playListWin, i+1, 1,"%s", playlist->getListOfSong()[i]->getTitle().c_str());

            }
            // mvwaddwstr(menuwin, i+1, 1, wide_title.c_str());

            // Highlight "Return to Main Menu" option
            if (highlight == number_of_songs) {
                wattron(playListWin, A_REVERSE);
                mvwprintw(playListWin, number_of_songs+1, 1, "Return to Main Menu");
                wattroff(playListWin, A_REVERSE);
            } else {
                mvwprintw(playListWin, number_of_songs+1, 1, "Return to Main Menu");
            }

            // Highlight "Play this playlist" option
            if (highlight == number_of_songs+1) {
                wattron(playListWin, A_REVERSE);
                mvwprintw(playListWin, number_of_songs+2, 1, "Play this playlist");
                wattroff(playListWin, A_REVERSE);
            } else {
                mvwprintw(playListWin, number_of_songs+2, 1, "Play this playlist");
            }

            wattroff(playListWin,A_REVERSE);
        }


        choice = wgetch(playListWin);
        if (choice == '\n'&& highlight == number_of_songs){
            werase(playListWin);
            wrefresh(playListWin);
            delwin(playListWin);
            return;
        }

        if (choice == '\n'&& highlight == number_of_songs+1){
            werase(playListWin);
            wrefresh(playListWin);

            int current = 0;

            std::thread th2([this,&current, &playlist](){
                // for(int i = 0; i<playlist->getListOfSong().size(); i++){
                //    current = i;
                //     //MusicPlayer::getInstance()->playMusic(playlist->getListOfSong()[i]->getFilePath().c_str());
                //     musicPlayer->initSDL();
                //     Song* selectedSOng = playlist->getListOfSong()[i];
                //     musicPlayer->playMusic((MusicModel::getInstance()->getPath() + "/" + playlist->getListOfSong()[i]->getFileName()).c_str());
                //     musicPlayer->cleanUpMusic();
                // }
                musicPlayer->initSDL();
                MusicPlayer::getInstance()->playMusicWithPlayList(playlist->getListOfSong());

            });
            // std::thread th3([this](){
            //     musicPlayer->checkMusicFinished();
            // });

            th2.detach();
          //  th3.detach();
            // delwin(playListWin);
            //endwin();
            //return;
             showPlayBackControlForPlayList(playlist->getListOfSong()[musicPlayer->getCurrentSongIndex()]);
            //playlist->play();
            //break;
            box(playListWin, 0,0); // Tao vien
            wrefresh(playListWin);



        }

        switch(choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight == number_of_songs+2)
                highlight = number_of_songs+1;
            break;
        case 10: // enter
            if (highlight != number_of_songs&& highlight != number_of_songs+1){
                werase(playListWin);
                wrefresh(playListWin);
                displaySongMetaData(playlist->getListOfSong()[highlight]);
                box(playListWin, 0,0); // Tao vien
            }

            break;
        default:
            break;
        }
        wrefresh(playListWin);
    }


    //printw("Your choice was: %s", choices[highlight].c_str());




}

void MusicController::showPlayLists()
{


}

void MusicController::viewPlayListControl()
{

}
void MusicController::setMetaDataOfSong(int i, Song * selectedSong)
{

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // Create a window for the playlist creation prompt
    WINDOW *updatedSongMetaDataWin = newwin(10, xMax - 12, (yMax / 2) - 5, (xMax / 2) - ((xMax - 12) / 2));
    box(updatedSongMetaDataWin, 0, 0);

    // Display instructions
    mvwprintw(updatedSongMetaDataWin, 1, 2, "=== UPDATE METADATA FIELD ===");
    mvwprintw(updatedSongMetaDataWin, 3, 2, "Enter  the new METADATA FIELD: ");
    wrefresh(updatedSongMetaDataWin);

    // Capture user input for the playlist name
    char nameBuffer[50];
    echo(); // Enable echo to show the user’s input
    mvwgetnstr(updatedSongMetaDataWin, 4, 4, nameBuffer, 50);
    noecho(); // Disable echo again after input

    TagLib::FileRef file((MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());
    if (i == MusicModel::getInstance()->getSongMetaIndex()["title"]){
        file.tag()->setTitle(nameBuffer);
        selectedSong->setTitle(std::string(nameBuffer));
        file.save();
    }
    if (i == MusicModel::getInstance()->getSongMetaIndex()["artist"]){
        file.tag()->setArtist(nameBuffer);
        selectedSong->setArtist(std::string(nameBuffer));
        file.save();
    }
    if (i == MusicModel::getInstance()->getSongMetaIndex()["album"]){
        file.tag()->setAlbum(nameBuffer);
        selectedSong->setAlbum(std::string(nameBuffer));
        file.save();
    }
    if (i == MusicModel::getInstance()->getSongMetaIndex()["genre"]){
        file.tag()->setGenre(nameBuffer);
        selectedSong->setGenre(std::string(nameBuffer));
        file.save();
    }


    // Confirmation message
    mvwprintw(updatedSongMetaDataWin, 6, 2, "Update song metadata successfully!");
    mvwprintw(updatedSongMetaDataWin, 7, 2, "Press any key to return to the previous menu...");
    wrefresh(updatedSongMetaDataWin);

    // Wait for user to press any key to return
    wgetch(updatedSongMetaDataWin);

    // Clean up
    delwin(updatedSongMetaDataWin);
    return;

}

void MusicController::playPlayList(Playlist* playlist)
{




}



void MusicController::showPlayBackControl(Song* selectedSong)
{

    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax);


    WINDOW* playBackControlWin = newwin(22, xMax -12, 5,5);

    box(playBackControlWin, 0, 0);

    // Hien thi thong tin metadata
    mvwprintw(playBackControlWin, 1,1, "==SONG METADATA==");
    mvwprintw(playBackControlWin, 2,1, "Title: %s", selectedSong->getTitle().c_str());
    mvwprintw(playBackControlWin, 3,1, "Artist: %s", selectedSong->getArtist().c_str());
    mvwprintw(playBackControlWin, 4,1, "Album: %s", selectedSong->getAlbum().c_str());
    mvwprintw(playBackControlWin, 6,1, "==SONG METADATA==");

    int duration = selectedSong->getDuration();
    int minutes = duration/60;
    int seconds = duration%60;
    mvwprintw(playBackControlWin, 5,1,"Duration: %d:%02d", minutes, seconds);


    //Display menu option
    std::string options[] = {"Play", "Pause/Resume", "Stop", "Increase Volume", "Decrease volume", "Return"};

    int highlight = 0;
    int choice;
    keypad(playBackControlWin, true);
    int currentPosition = 0;

    while (true) {
        for (int i = 0; i<6; ++i){
            if (i == highlight){
                wattron(playBackControlWin, A_REVERSE);
            }
            mvwprintw(playBackControlWin, 8+i,1, "%s", options[i].c_str() );
            wattroff(playBackControlWin, A_REVERSE);
        }
        currentPosition = musicPlayer->getCurrentPos();
        minutes = currentPosition/60;
        seconds = currentPosition%60;
        mvwprintw(playBackControlWin, 15,1,"Current position of song: %d:%02d", minutes, seconds);
        mvwprintw(playBackControlWin, 16,1,"Current volumne of song---:   %d",musicPlayer->getVolume());


        choice = wgetch(playBackControlWin);
        if (choice =='\n' && highlight == 0) {
            werase(playBackControlWin);
            wrefresh(playBackControlWin);
            musicPlayer->initSDL();
            //std::thread th1(&MusicPlayer::playMusic(), musicPlayer, (MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());
            //musicPlayer->playMusic((MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());
            std::thread th1([this, selectedSong]() {
                musicPlayer->playMusic((MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());

            });

            th1.detach();
            // musicPlayer->cleanUpSDL();
            box(playBackControlWin,0,0);



        };
        if (choice =='\n' && highlight == 1) {
            musicPlayer->togglePlay();
        };
        if (choice =='\n' && highlight == 2) {

            musicPlayer->stopMusic();
        };
        if (choice =='\n' && highlight == 3) {

            musicPlayer->increaseVolume();
        };
        if (choice =='\n' && highlight == 4) {

            musicPlayer->decreaseVolume();
        };



        if (choice =='\n' && highlight == 5) {
            werase(playBackControlWin);
            wrefresh(playBackControlWin);
            delwin(playBackControlWin);
            return;
        };

        // if (choice == '\n' && highlight == 2){
        //     werase(playBackControlWin);
        //     wrefresh(playBackControlWin);

        //     box(playBackControlWin, 0,0); // Tao vien
        //     mvwprintw(playBackControlWin, 12, 1,"Stop song successfully");
        // }

        //Handle navigation and selection
        switch(choice){
        case KEY_UP:
            highlight = (highlight ==0)? 5: highlight -1;
            break;
        case KEY_DOWN:
            highlight = (highlight == 5)? 0: highlight +1;
        case 10:
            if(highlight == 0){
                // Play song

            }
            else if (highlight == 1){
                // Pause/ Resume
            }
            else if (highlight == 2){
                // Stop

            }
            else if (highlight == 5){
                // Return
            }
            break;
        default:
            break;
        }
        wrefresh(playBackControlWin);
    }




}

void MusicController::showPlayBackControlForPlayList(Song* selectedSong)
{

    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax);


    WINDOW* playBackControlWin = newwin(22, xMax -12, 5,5);

    box(playBackControlWin, 0, 0);

    // Hien thi thong tin metadata
    mvwprintw(playBackControlWin, 1,1, "==SONG METADATA==");
    mvwprintw(playBackControlWin, 2,1, "Title: %s", selectedSong->getTitle().c_str());
    mvwprintw(playBackControlWin, 3,1, "Artist: %s", selectedSong->getArtist().c_str());
    mvwprintw(playBackControlWin, 4,1, "Album: %s", selectedSong->getAlbum().c_str());
    mvwprintw(playBackControlWin, 6,1, "==SONG METADATA==");

    int duration = selectedSong->getDuration();
    int minutes = duration/60;
    int seconds = duration%60;
    mvwprintw(playBackControlWin, 5,1,"Duration: %d:%02d", minutes, seconds);


    //Display menu option
    std::string options[] = {"Play", "Pause/Resume", "Stop", "Increase Volume", "Decrease volume", "Previous", "Next", "Return", };

    int highlight = 0;
    int choice;
    keypad(playBackControlWin, true);
    int currentPosition = 0;

    while (true) {
        for (int i = 0; i<8; ++i){
            if (i == highlight){
                wattron(playBackControlWin, A_REVERSE);
            }
            mvwprintw(playBackControlWin, 8+i,1, "%s", options[i].c_str() );
            wattroff(playBackControlWin, A_REVERSE);
        }
        currentPosition = musicPlayer->getCurrentPos();
        minutes = currentPosition/60;
        seconds = currentPosition%60;
        mvwprintw(playBackControlWin, 17,1,"Current position of song: %d:%02d", minutes, seconds);
        mvwprintw(playBackControlWin, 18,1,"Current volumne of song---:   %d",musicPlayer->getVolume());


        choice = wgetch(playBackControlWin);
        if (choice =='\n' && highlight == 0) {
            musicPlayer->initSDL();
            // std::thread th1(&MusicPlayer::playMusic(), musicPlayer, (MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());
            //musicPlayer->playMusic((MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());
            std::thread th1([this, selectedSong]() {
                musicPlayer->playMusic((MusicModel::getInstance()->getPath() + "/" + selectedSong->getFileName()).c_str());

            });

            th1.detach();
            // musicPlayer->cleanUpSDL();


        };
        if (choice =='\n' && highlight == 1) {
            musicPlayer->togglePlay();
        };
        if (choice =='\n' && highlight == 2) {

            musicPlayer->stopMusic();
        };
        if (choice =='\n' && highlight == 3) {

            musicPlayer->increaseVolume();
        };
        if (choice =='\n' && highlight == 4) {

            musicPlayer->decreaseVolume();
        };
        if (choice =='\n' && highlight == 5) {

            std::thread t5( [&, this](){   musicPlayer->previousSong();});
            t5.detach();
          //  musicPlayer->previousSong();

        };
        if (choice =='\n' && highlight == 6) {
            // werase(playBackControlWin);
            // wrefresh(playBackControlWin);
            std::thread t4( [&selectedSong, this](){selectedSong = musicPlayer->nextSong();});
            t4.detach();
           // delwin(playBackControlWin);
           // return;

        };





        if (choice =='\n' && highlight == 7) {
            werase(playBackControlWin);
            wrefresh(playBackControlWin);
            delwin(playBackControlWin);
            return;
        };

        if (choice == '\n' && highlight == 2){
            werase(playBackControlWin);
            wrefresh(playBackControlWin);

            box(playBackControlWin, 0,0); // Tao vien
            mvwprintw(playBackControlWin, 12, 1,"Stop song successfully");
        }

        //Handle navigation and selection
        switch(choice){
        case KEY_UP:
            highlight = (highlight ==0)? 7: highlight -1;
            break;
        case KEY_DOWN:
            highlight = (highlight == 7)? 0: highlight +1;
        case 10:
            if(highlight == 0){
                // Play song

            }
            else if (highlight == 1){
                // Pause/ Resume
            }
            else if (highlight == 2){
                // Stop

            }
            else if (highlight == 5){
                // Return
            }
            break;
        default:
            break;
        }
        wrefresh(playBackControlWin);
    }




}




void MusicController::showPlayListMenu()
{
    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax);


    WINDOW* playListMenuWin = newwin(20, xMax -12, 5,5);

    box(playListMenuWin, 0, 0);

    //Display menu option
    std::string options[] = {"1. Create a new playlist", "2. Update a playlist", "3. List of Playlists", "4. View a Playlist", "5. Delete a playlist", "6. Return to a media menu"};

    int highlight = 0;
    int choice;
    keypad(playListMenuWin, true);

    while (true) {
        for (int i = 0; i<6; ++i){
            if (i == highlight){
                wattron(playListMenuWin, A_REVERSE);
            }
            mvwprintw(playListMenuWin, 8+i,1, "%s", options[i].c_str() );
            wattroff(playListMenuWin, A_REVERSE);
        }

        choice = wgetch(playListMenuWin);
        if (choice =='\n' && highlight == 5) {
            werase(playListMenuWin);
            wrefresh(playListMenuWin);
            delwin(playListMenuWin);
            return;

        };
        if (choice =='\n' && highlight == 0) {
            werase(playListMenuWin);
            wrefresh(playListMenuWin);
            playlistController->createPlayList();
            box(playListMenuWin, 0,0); // Tao vien
        };


        if (choice == '\n' && highlight == 1){
            werase(playListMenuWin);
            wrefresh(playListMenuWin);
            playlistController->updatePlayList();
            box(playListMenuWin,0,0);

        }
        // show list of playlist
        if (choice == '\n' && highlight == 2){
            werase(playListMenuWin);
            wrefresh(playListMenuWin);
            playlistController->showListOfPlayList();
            box(playListMenuWin, 0,0); // Tao vien
        }
        // view a playlist
        if (choice == '\n' && highlight == 3){
            werase(playListMenuWin);
            wrefresh(playListMenuWin);
            viewPlayList();
            box(playListMenuWin,0,0);

        }
        // Delete a playlist
        if (choice == '\n' && highlight == 4){
            werase(playListMenuWin);
            wrefresh(playListMenuWin);
            playlistController->deletePlayList();
            box(playListMenuWin,0,0);

        }

        //Handle navigation and selection
        switch(choice){
        case KEY_UP:
            highlight = (highlight ==0)? 5: highlight -1;
            break;
        case KEY_DOWN:
            highlight = (highlight == 5)? 0: highlight +1;
        case 10:
            if(highlight == 0){
                //1. Create a new playlist

            }
            else if (highlight == 1){
                //2. Update a playlist
            }
            else if (highlight == 2){
                // 3. List of playLists

            }
            else if (highlight == 3){
                // 4. View a playLists
            }
            else if (highlight == 4)
            {
                // 5. Delete a playlist
            }
            break;
        default:
            break;
        }
        wrefresh(playListMenuWin);
    }
}

void MusicController::viewPlayList() // giong nhu hay showMediaFile
{

    // Set the locale to support UTF-8 encoding
    setlocale(LC_ALL, "vi_VN.UTF-8");

    // Set the terminal output encoding to UTF-8
    //std::cout.imbue(std::locale("vi_VN.UTF-8"));
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_playlist = (int)playlistController->getListOfPlaylist().size();
    std::cout<<"number of PlayList: "<<number_of_playlist;
    WINDOW* playListWin = newwin(number_of_playlist + 3,xMax-12,5,5);
    box(playListWin, 0, 0);
    refresh();
    wrefresh(playListWin);
    keypad(playListWin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...

    int choice;
    int highlight = 0;

    // su dung vong lap while de hien thi menu
    while(1){
        for(int i = 0; i<number_of_playlist; i++){
            if(i == highlight)
                wattron(playListWin, A_REVERSE);
            // Muc dich: de fix loi phong chu khi hien thi tieng viet ==> Nhung chua xu ly dc

            mvwprintw(playListWin, i+1, 1, "%s", playlistController->getListOfPlaylist()[i]->getPlayListName().c_str());
            // mvwaddwstr(menuwin, i+1, 1, wide_title.c_str());

            wattroff(playListWin,A_REVERSE);

            if (highlight == number_of_playlist){ // la
                wattron(playListWin, A_REVERSE);
            }
            if (i == number_of_playlist -1)
                mvwprintw(playListWin, i+2, 1,"Final. Return to Main Menu");


            wattroff(playListWin,A_REVERSE);


        }

        choice = wgetch(playListWin);
        if (choice == '\n'&& highlight == number_of_playlist){
            werase(playListWin);
            wrefresh(playListWin);
            delwin(playListWin);
            return;
        }

        switch(choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight == number_of_playlist+1)
                highlight = number_of_playlist;
            break;
        case 10: // enter
            if (highlight != number_of_playlist){
                werase(playListWin);
                wrefresh(playListWin);
                viewPlayList(playlistController->getListOfPlaylist()[highlight]);
                box(playListWin, 0, 0);
            }
            break;
        default:
            break;
        }
        wrefresh(playListWin);
    }

    // Wait for user to press any key to return
    wgetch(playListWin);

    // Clean up
    delwin(playListWin);
    //printw("Your choice was: %s", choices[highlight].c_str());
    return;

}


void MusicController::editSongMetaData(Song* selectedSong)
{
    int yMax, xMax;

    getmaxyx(stdscr, yMax, xMax);


    WINDOW* songMetaDataWin = newwin(20, xMax -12, 5,5);

    box(songMetaDataWin, 0, 0);

    // Hien thi thong tin metadata
    mvwprintw(songMetaDataWin, 1,1, "==SONG METADATA==");
    mvwprintw(songMetaDataWin, 2,1, "Title: %s", selectedSong->getTitle().c_str());
    mvwprintw(songMetaDataWin, 3,1, "Artist: %s", selectedSong->getArtist().c_str());
    mvwprintw(songMetaDataWin, 4,1, "Album: %s", selectedSong->getAlbum().c_str());
    mvwprintw(songMetaDataWin, 6,1, "==SONG METADATA==");

    int duration = selectedSong->getDuration();
    int minutes = duration/60;
    int seconds = duration%60;
    mvwprintw(songMetaDataWin, 5,1,"Duration: %d:%02d", minutes, seconds);




    //Display menu option
    std::string options[] = {"Title", "Artist", "Album","Genre", "Return"};




    int highlight = 0;
    int choice;
    keypad(songMetaDataWin, true);

    while (true) {
        for (int i = 0; i<5; ++i){
            if (i == highlight){
                wattron(songMetaDataWin, A_REVERSE);
            }
            mvwprintw(songMetaDataWin, 8+i,1, "%s", options[i].c_str() );
            wattroff(songMetaDataWin, A_REVERSE);
        }

        choice = wgetch(songMetaDataWin);
        if (choice =='\n' && highlight == 4) {
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            delwin(songMetaDataWin);
            return;
        };
        if (choice == '\n' && highlight == 3){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            setMetaDataOfSong(3, selectedSong);
            box(songMetaDataWin, 0,0); // Tao vien
            mvwprintw(songMetaDataWin, 16, 1,"Add Song Successfully");

        };
        if (choice == '\n' && highlight == 2){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            setMetaDataOfSong(2, selectedSong);
            box(songMetaDataWin, 0,0); // Tao vien
            mvwprintw(songMetaDataWin, 16, 1,"Add Song Successfully");


        };

        if (choice == '\n' && highlight == 1){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            setMetaDataOfSong(1, selectedSong);
            box(songMetaDataWin, 0,0); // Tao vien
            mvwprintw(songMetaDataWin, 16, 1,"Add Song Successfully");

        }

        if (choice == '\n' && highlight == 0){
            werase(songMetaDataWin);
            wrefresh(songMetaDataWin);
            setMetaDataOfSong(0, selectedSong);
            box(songMetaDataWin, 0,0); // Tao vien
            mvwprintw(songMetaDataWin, 16, 1,"Add Song Successfully");


        }
        //Handle navigation and selection
        switch(choice){
        case KEY_UP:
            highlight = (highlight ==0)? 4: highlight -1;
            break;
        case KEY_DOWN:
            highlight = (highlight == 4)? 0: highlight +1;
        case 10:
            if(highlight == 0){
            }
            else if (highlight == 1){
            }
            else if (highlight == 2){
            }
            else if (highlight == 3){
            }
            break;
        default:
            break;
        }
        wrefresh(songMetaDataWin);


    }



}

