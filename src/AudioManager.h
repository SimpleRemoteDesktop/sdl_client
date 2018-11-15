//
// Created by user on 11/13/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_AUDIOMANAGER_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_AUDIOMANAGER_H


#include "Frame.h"
#include "Queue.h"

class AudioManager {
public:
    AudioManager(Queue<Frame> * audio, int frameRate, int channels);
    void run();
private:
    Queue<Frame> *queue;
    int framerate;
    int channels;
    bool isRunning;
};


#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_AUDIOMANAGER_H
