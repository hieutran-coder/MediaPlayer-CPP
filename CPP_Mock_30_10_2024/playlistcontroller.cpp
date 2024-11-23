#include "playlistcontroller.h"
#include<iostream>
#include<ncurses.h>
#include"musicmodel.h"
PlayListController::PlayListController() {}

PlayListController *PlayListController::getInstance()
{
    static PlayListController instance;
    return &instance;


}

void PlayListController::createPlayList()
{

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // Create a window for the playlist creation prompt
    WINDOW *createPlaylistWin = newwin(10, xMax - 12, (yMax / 2) - 5, (xMax / 2) - ((xMax - 12) / 2));
    box(createPlaylistWin, 0, 0);

    // Display instructions
    mvwprintw(createPlaylistWin, 1, 2, "=== CREATE A NEW PLAYLIST ===");
    mvwprintw(createPlaylistWin, 3, 2, "Enter the name of the new Playlist: ");
    wrefresh(createPlaylistWin);

    // Capture user input for the playlist name
    char nameBuffer[50];
    echo(); // Enable echo to show the user’s input
    mvwgetnstr(createPlaylistWin, 4, 4, nameBuffer, 50);
    noecho(); // Disable echo again after input

    // kiem tra xem co trung ten voi cac playlist truoc do khoong?
    for(int i = 0; i<listOfPlaylist.size(); i++){
        if (std::string(nameBuffer) == listOfPlaylist[i]->getPlayListName()) return;
    }

    // Store the new playlist
    Playlist *newPlayList = new Playlist();
    newPlayList->setPlayListName(nameBuffer);
    listOfPlaylist.push_back(newPlayList);

    // Confirmation message
    mvwprintw(createPlaylistWin, 6, 2, "Playlist created successfully!");
    mvwprintw(createPlaylistWin, 7, 2, "Press any key to return to the previous menu...");
    wrefresh(createPlaylistWin);

    // Wait for user to press any key to return
    wgetch(createPlaylistWin);

    // Clean up
    delwin(createPlaylistWin);
    return;
}

void PlayListController::showListOfPlayList() // giong nhu hay showMediaFile
{

    // Set the locale to support UTF-8 encoding
    setlocale(LC_ALL, "vi_VN.UTF-8");

    // Set the terminal output encoding to UTF-8
    //std::cout.imbue(std::locale("vi_VN.UTF-8"));
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_playlist = (int)listOfPlaylist.size();
    std::cout<<"number_of_playlist: "<<number_of_playlist;
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

            mvwprintw(playListWin, i+1, 1,"%s", listOfPlaylist[i]->getPlayListName().c_str());
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

void PlayListController::addToPlayList(Song *song)  // Giong nhu ham showListOfPlayList. Nhung no van da bi loi, va lap code nhieu qua
{
    // Set the locale to support UTF-8 encoding
    setlocale(LC_ALL, "vi_VN.UTF-8");

    // Set the terminal output encoding to UTF-8
    //std::cout.imbue(std::locale("vi_VN.UTF-8"));
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_playlist = (int)listOfPlaylist.size();
    std::cout<<"number_of_playlist: "<<number_of_playlist;
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

            mvwprintw(playListWin, i+1, 1,"%s", listOfPlaylist[i]->getPlayListName().c_str());
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
                listOfPlaylist[highlight]->addSong(song);
                mvwprintw(playListWin, number_of_playlist+2, 1,"====Add Song Successfully====");
                delwin(playListWin);
                return;
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


void PlayListController::updatePlayList()
{
    // Set the locale to support UTF-8 encoding
    setlocale(LC_ALL, "vi_VN.UTF-8");

    // Set the terminal output encoding to UTF-8
    //std::cout.imbue(std::locale("vi_VN.UTF-8"));
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_playlist = (int)listOfPlaylist.size();
    std::cout<<"number_of_playlist: "<<number_of_playlist;
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

            mvwprintw(playListWin, i+1, 1,"%s", listOfPlaylist[i]->getPlayListName().c_str());
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
                updatePlayList(listOfPlaylist[highlight]);
                box(playListWin, 0,0);
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

void PlayListController::updatePlayList(Playlist *playlist)
{
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_songs = (int)playlist->getListOfSong().size();
    WINDOW* playListWin = newwin(number_of_songs + 10,xMax-12,5,5);
    box(playListWin, 0, 0);
    refresh();
    wrefresh(playListWin);
    keypad(playListWin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...


    int choice;
    int highlight = 0;

    // su dung vong lap while de hien thi menu
    for(int i = 0; i<number_of_songs; i++){
       ;
        // Muc dich: de fix loi phong chu khi hien thi tieng viet ==> Nhung chua xu ly dc
        std::wstring wide_title;
        mvwprintw(playListWin, i+1, 1,"%s", playlist->getListOfSong()[i]->getTitle().c_str());
        // mvwaddwstr(menuwin, i+1, 1, wide_title.c_str());
    }

    std::string options[] = {"1. Add Song", "2. Remove Song", "3. Order songs by title, duration,...", "4. Change the playlist name", "5. Return"};

    while(true){
        for (int i = 0; i<5; i++){
            if (i == highlight)
            {
                wattron(playListWin, A_REVERSE);
            }

            mvwprintw(playListWin, number_of_songs+i+3,1, "%s", options[i].c_str() );
            wattroff(playListWin, A_REVERSE);
        }

        choice = wgetch(playListWin);
        if (choice == '\n'&& highlight == 0){
            werase(playListWin);
            wrefresh(playListWin);
            chooseSongFromMediaToAdd(playlist);
            mvwprintw(playListWin, number_of_songs + 15, 1,"=== Succcessfull add song to this playlist ===");
            box(playListWin, 0,0);
        }
        if (choice == '\n'&& highlight == 1){
            werase(playListWin);
            wrefresh(playListWin);
            chooseSongToRemove(playlist);
            mvwprintw(playListWin, number_of_songs + 15, 1,"=== Succcessfull remove song to this playlist ===");
            box(playListWin, 0,0);
        }
        if (choice == '\n'&& highlight == 3){
            werase(playListWin);
            wrefresh(playListWin);
            renameOfPlayList(playlist);
            box(playListWin, 0,0);
        }
        if (choice == '\n'&& highlight == 4){
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
            if(highlight == 5)
                highlight = 4;
            break;
        case 10: // enter

            break;
        default:
            break;
        }
        wrefresh(playListWin);
    }


    //printw("Your choice was: %s", choices[highlight].c_str());

    getch();
    endwin();
}




void PlayListController::chooseSongFromMediaToAdd(Playlist* playlist){  // Tai su dung code display list songs

    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_songs = (int)MusicModel::getInstance()->getSize();
    std::cout<<"number of song: "<<number_of_songs;
    WINDOW* menuwin = newwin(number_of_songs + 3,xMax-12,5,5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...

    int choice;
    int highlight = 0;

    // su dung vong lap while de hien thi menu
    while(1){
        for(int i = 0; i<number_of_songs; i++){
            if(i == highlight)
                wattron(menuwin, A_REVERSE);
            // Muc dich: de fix loi phong chu khi hien thi tieng viet ==> Nhung chua xu ly dc

            mvwprintw(menuwin, i+1, 1,"%s", MusicModel::getInstance()->getListSongs()[i]->getTitle().c_str());
            // mvwaddwstr(menuwin, i+1, 1, wide_title.c_str());

            wattroff(menuwin,A_REVERSE);

            if (highlight == number_of_songs){ // la
                wattron(menuwin, A_REVERSE);
            }
            if (i == number_of_songs -1)
                mvwprintw(menuwin, i+2, 1,"Final. Return to Main Menu");
            wattroff(menuwin,A_REVERSE);

        }

        choice = wgetch(menuwin);
        if (choice == '\n'&& highlight == number_of_songs){
            werase(menuwin);
            wrefresh(menuwin);
            delwin(menuwin);
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
            if(highlight == number_of_songs+1)
                highlight = number_of_songs;
            break;
        case 10: // enter
            if (highlight != number_of_songs){
                werase(menuwin);
                wrefresh(menuwin);
                playlist->addSong(MusicModel::getInstance()->getListSongs()[highlight]);
                mvwprintw(menuwin, number_of_songs + 5, 1, "=== Succcessfull add song to this playlist ===");
                box(menuwin, 0,0); // Tao vien
            }
            break;
        default:
            break;
        }
        wrefresh(menuwin);
    }


    //printw("Your choice was: %s", choices[highlight].c_str());

    getch();
    endwin();
}


void PlayListController::chooseSongToRemove(Playlist* playlist){  // Tai su dung code display list songs

    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_songs = (int)playlist->getListOfSong().size();
    std::cout<<"number of song: "<<number_of_songs;
    WINDOW* menuwin = newwin(number_of_songs + 3,xMax-12,5,5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin, true); // cho phep inputwin nhan cac ki tu dac biet nhu KEY_UP, KEY_DOWN,...

    int choice;
    int highlight = 0;

    // su dung vong lap while de hien thi menu
    while(1){
        for(int i = 0; i<number_of_songs; i++){
            if(i == highlight)
                wattron(menuwin, A_REVERSE);
            // Muc dich: de fix loi phong chu khi hien thi tieng viet ==> Nhung chua xu ly dc

            mvwprintw(menuwin, i+1, 1,"%s", playlist->getListOfSong()[i]->getTitle().c_str());
            // mvwaddwstr(menuwin, i+1, 1, wide_title.c_str());

            wattroff(menuwin,A_REVERSE);

            if (highlight == number_of_songs){ // la
                wattron(menuwin, A_REVERSE);
            }
            if (i == number_of_songs -1)
                mvwprintw(menuwin, i+2, 1,"Final. Return to Main Menu");
            wattroff(menuwin,A_REVERSE);

        }

        choice = wgetch(menuwin);
        if (choice == '\n'&& highlight == number_of_songs){
            werase(menuwin);
            wrefresh(menuwin);
            delwin(menuwin);
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
            if(highlight == number_of_songs+1)
                highlight = number_of_songs;
            break;
        case 10: // enter
            if (highlight != number_of_songs){
                werase(menuwin);
                wrefresh(menuwin);
                playlist->removeSong(playlist->getListOfSong()[highlight]);
                number_of_songs --;
                mvwprintw(menuwin, number_of_songs + 5, 1, "=== Succcessfull add song to this playlist ===");
                box(menuwin, 0,0); // Tao vien
            }
            break;
        default:
            break;
        }
        wrefresh(menuwin);
    }
    //printw("Your choice was: %s", choices[highlight].c_str());
    getch();
    endwin();

}



void PlayListController::renameOfPlayList(Playlist *playlist)
{

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // Create a window for the playlist creation prompt
    WINDOW *renamedPlaylistWin = newwin(10, xMax - 12, (yMax / 2) - 5, (xMax / 2) - ((xMax - 12) / 2));
    box(renamedPlaylistWin, 0, 0);

    // Display instructions
    mvwprintw(renamedPlaylistWin, 1, 2, "=== RENAME THE PLAYLIST ===");
    mvwprintw(renamedPlaylistWin, 3, 2, "Enter the new name of the Playlist: ");
    wrefresh(renamedPlaylistWin);

    // Capture user input for the playlist name
    char nameBuffer[50];
    echo(); // Enable echo to show the user’s input
    mvwgetnstr(renamedPlaylistWin, 4, 4, nameBuffer, 50);
    noecho(); // Disable echo again after input

    playlist->setPlayListName(nameBuffer);

    // Confirmation message
    mvwprintw(renamedPlaylistWin, 6, 2, "Playlist name was changed successfully!");
    mvwprintw(renamedPlaylistWin, 7, 2, "Press any key to return to the previous menu...");
    wrefresh(renamedPlaylistWin);

    // Wait for user to press any key to return
    wgetch(renamedPlaylistWin);

    // Clean up
    delwin(renamedPlaylistWin);
    return;

}

void PlayListController::deletePlayList() // Tai su dung tu showListOfPlayList code
{
    int y, x, yBeg, xBeg, yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int number_of_playlist = (int)listOfPlaylist.size();
    std::cout<<"number_of_playlist: "<<number_of_playlist;
    WINDOW* playListWin = newwin(number_of_playlist + 10,xMax-12,5,5);
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

            mvwprintw(playListWin, i+1, 1,"%s", listOfPlaylist[i]->getPlayListName().c_str());
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
                    listOfPlaylist.erase(listOfPlaylist.begin() + highlight);
                     number_of_playlist--;
                    mvwprintw(playListWin, number_of_playlist+3, 2, "Playlist was remove successfully!");
                    mvwprintw(playListWin, number_of_playlist+4, 2, "Press any key to return to the previous menu...");
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
std::vector<Playlist *> PlayListController::getListOfPlaylist() const
{
    return listOfPlaylist;
}

void PlayListController::setListOfPlaylist(const std::vector<Playlist *> &newListOfPlaylist)
{
    listOfPlaylist = newListOfPlaylist;
}




