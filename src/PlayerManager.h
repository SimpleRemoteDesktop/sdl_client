//
// Created by user on 11/12/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_APPMANAGER_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_APPMANAGER_H


#include <SDL2/SDL_video.h>
#include <string>
#include "network.h"
#include "VideoManager.h"
#include "AudioManager.h"

class InputHandler;
    
enum screen_dimension { SRD_WINDOW, SRD_FULLSCREEN, SRD_FULLSCREEN_WINDOW };

class PlayerManager {

public:
    void fullScreenHandler();
    PlayerManager(std::string hostname, int port, int codecWidth, int codecHeight, int bandwidth, int fps, bool withRelativeMouse);
    ~PlayerManager();
    void getScreenSize(int *pInt, int *pInt1);
    void start();
    void quit();

private:
    enum screen_dimension screenDimension = SRD_WINDOW;
    SDL_Window *screen;
    std::string hostname;
    int port;
    int codecWidth;
    int codecHeight;
    int bandwidth;
    int fps;
    Network *network;
    Queue<Frame> *videoQueue;
    Queue<Frame> *audioQueue;
    VideoManager *videoMgr;
    SDL_Thread *videoThread;
    AudioManager *audioMgr;
    SDL_Thread *audioThread;
    InputHandler *input;
    SDL_Thread *networkThread;
    SDL_Thread *inputThread;
};

#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_APPMANAGER_H
