//
// Created by user on 11/12/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_VIDEOMANAGER_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_VIDEOMANAGER_H


#include <SDL2/SDL_video.h>
#include "Queue.h"
#include "Frame.h"
#include "video_surface.h"
#include "video_decoder.h"


class VideoManager {
public:
    VideoManager(Queue<Frame> * videoQueue, int screenWidth, int screenHeight, SDL_Window* screen);
    ~VideoManager();
    void run();

private:
    Queue<Frame> *videoQueue;
    SdlVideoRenderer *surface;
    int screenWidth;
    int screenHeight;
    SoftwareVideoDecoder *decoder;
    bool isRunning;
    SDL_Window *screen;
};


#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_VIDEOMANAGER_H
