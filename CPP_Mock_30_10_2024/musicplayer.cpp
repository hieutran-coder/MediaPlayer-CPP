#include "musicplayer.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<cstdlib>


// thread: code prompt from GPT

MusicPlayer::MusicPlayer() {}

MusicPlayer *MusicPlayer::getInstance(){
    static MusicPlayer instance;
    return &instance;
}



int MusicPlayer::loadMusic(std::string fileName){
    Mix_Music *m = NULL;
    m = Mix_LoadMUS(fileName.c_str());
    if(m == NULL){
        printf("Fail to load Music. SDL_Mixer Error: %s\n", Mix_GetError());
        return -1;
    }
    lMusic.push_back(m);
    return lMusic.size() -1;
}

int MusicPlayer::getVolume() const
{
    return volume;
}
///////
///
void MusicPlayer::increaseVolume()
{
    std::lock_guard<std::mutex> lock(mutex);
    volume++;
    //std::system("amixer set master 10%+ > /dev/null 2>&1"); // change volume of system " > /dev/null 2>&1" to redirects output to terminal
    volumeChanged = true;
    cv.notify_one();
}

void MusicPlayer::decreaseVolume()
{
    std::lock_guard<std::mutex> lock(mutex);
    volume--;
    // std::system("amixer set Master 10%- > /dev/null 2>&1");
    volumeChanged = true;
    cv.notify_one();
}




void MusicPlayer::setVolume(int v){
    volume = (MIX_MAX_VOLUME*v)/100;
}

int MusicPlayer::playMusic(int m){
    if (Mix_PlayingMusic() ==0){
        Mix_Volume(1, volume);
        Mix_PlayMusic(lMusic[m], -1);
    }
    return 0;

}



void MusicPlayer::initSDL(){
    if(SDL_Init(SDL_INIT_AUDIO)<0){
        std::cerr<<"SDL could not initialize! SDL_Error: "<< SDL_GetError()<<std::endl;
        exit(1);
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4048) == -1){
        std::cerr<<"SDL_mixer could not initialize! SDL_Error: "<< Mix_GetError()<<std::endl;
        exit(1);
    }
    setVolume(80);
}

void MusicPlayer::quitMixer(){

    for(int i = 0; i<lMusic.size(); i++){
        Mix_FreeMusic(lMusic[i]);
        lMusic[i] = NULL;
    }
    Mix_Quit();
}

void MusicPlayer::togglePlay(){


    std::lock_guard<std::mutex> lock(mutex);
    shouldToggle = true;
    cv.notify_one();
}

void MusicPlayer::stopMusic(){
    std::lock_guard<std::mutex> lock(mutex);
    shouldStop = true;
    cv.notify_one();
}





void MusicPlayer::cleanUpMusic(){
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }
    for(int i = 0; i<lMusic.size(); i++){
        Mix_FreeMusic(lMusic[i]);
        lMusic[i] = NULL;
    }
    lMusic.clear();
}


void MusicPlayer::cleanUpSDL(){
    currentSongIndex = -1;
    Mix_CloseAudio();
    SDL_Quit();
}



int MusicPlayer::getCurrentPos()
{
    currentPos = Mix_GetMusicPosition(music);
    return currentPos;
}

void MusicPlayer::setCurrentPos(int newCurrentPos)
{
    currentPos = newCurrentPos;
}



/// Y tuong tu chatGPT + claude AI prompts
void MusicPlayer::playMusic(const char* filePath){

    music = Mix_LoadMUS(filePath); // load audio from file
    if (!music){
        std::cerr <<"Failed to load music: "<<Mix_GetError()<<std::endl;
        return;
    }

    if(Mix_PlayMusic(music, -1) == -1){
        std::cerr<<"Mix_PlayMusic error"<<Mix_GetError()<<std::endl;
        Mix_FreeMusic(music);
        return;
    }
    bool isPause = false;
    while(true){

        std::unique_lock<std::mutex> lock(mutex);

        currentPos = Mix_GetMusicPosition(music);
        \
        cv.wait(lock, [this]{ return shouldToggle|| shouldStop||volumeChanged;} );

        if (shouldStop){
            if(Mix_PlayingMusic() !=0)
                 Mix_HaltMusic();
            if(!music)
                Mix_FreeMusic(music);
            //cleanUpSDL();
            return;
        }

        if (shouldToggle){
            if(isPause){
                Mix_ResumeMusic();
            } else{
                Mix_PauseMusic();
            }
            isPause = !isPause;
            shouldToggle = false;

        }

        if (volumeChanged){
            Mix_VolumeMusic(volume);
            Mix_Volume(-1, volume);
            volumeChanged = false;
        }



    }

}
void MusicPlayer::onMusicFinished()
{


    std::cout<<"Song finish playing"<<std::endl;
    MusicPlayer:: getInstance()->nextSong();
}

void MusicPlayer::checkMusicFinished(){
    while(1){
        if (Mix_PlayingMusic() ==0){
            std::lock_guard<std::mutex> lock(mutex);
            musicFinished = true;
            cv.notify_one();
            break;
        }

    }

}

int MusicPlayer::getCurrentSongIndex() const
{
    return currentSongIndex;
}


void MusicPlayer::previousSong(){
    //   std::lock_guard<std::mutex> lock(mutex);
   // stopMusic();
    if(!lMusic.empty()){
         currentSongIndex = (currentSongIndex - 1) % lMusic.size();
        playCurrentSong();
    }
}

Song* MusicPlayer::nextSong(){
    //std::lock_guard<std::mutex> lock(mutex);
    //stopMusic();
    if (!lMusic.empty()){
        currentSongIndex = (currentSongIndex + 1) % lMusic.size();
        playCurrentSong();
    }

    return listOfSongs[currentSongIndex];
}




int MusicPlayer::playCurrentSong(){
    if (lMusic.empty()) return -1 ;


    if(Mix_PlayMusic(lMusic[currentSongIndex], 0) == -1){
        std::cerr<<"Mix_PlayMusic error"<<Mix_GetError()<<std::endl;
        Mix_FreeMusic(music);
        return 0;
    }
    // Create a new thread to continuously check if the music has finished playing
    std::thread musicFinishCheckThread(&MusicPlayer::checkMusicFinished, this);
    musicFinishCheckThread.detach();
   // Mix_HookMusicFinished(onMusicFinished);
    music = lMusic[currentSongIndex];
    bool isPause = false;
    while(true){
        std::unique_lock<std::mutex> lock(mutex);
        currentPos = Mix_GetMusicPosition(music);




        cv.wait(lock, [this]{

            return shouldToggle|| shouldStop||volumeChanged||musicFinished;} );

        if (musicFinished)
        {

            nextSong();
            break;
        }

        if (shouldStop){
            Mix_HaltMusic();
            Mix_FreeMusic(music);
            currentSongIndex = -1;
             return 0;
        }

        if (shouldToggle){
            if(isPause){
                Mix_ResumeMusic();
            } else{
                Mix_PauseMusic();
            }
            isPause = !isPause;
            shouldToggle = false;

        }

        if (volumeChanged){
            Mix_VolumeMusic(volume);
            Mix_Volume(-1, volume);
            volumeChanged = false;
        }



    }


};

int MusicPlayer::playMusicWithPlayList(std::vector<Song*> listOfSong){

    currentSongIndex = 0;
    cleanUpMusic();
    listOfSongs = listOfSong;
    for(int i = 0; i<listOfSong.size(); i++){
        music = Mix_LoadMUS( (MusicModel::getInstance()->getPath() + "/" +   listOfSong[i]->getFileName()).c_str()); // load audio from file
        if (!music){
            std::cerr <<"Failed to load music: "<<Mix_GetError()<<std::endl;
            return 0;
        }
        lMusic.push_back(music);
    }

    playCurrentSong();



}

