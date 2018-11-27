//
// Created by user on 11/6/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_FRAME_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_FRAME_H


#include <stdint-gcc.h>

enum FRAME_TYPE {
    VIDEO_FRAME = 1, AUDIO_FRAME = 2, DIMENSION_FRAME = 3
};

class Frame {
public:
    Frame() {

    }

    bool quit;
    uint8_t *data;
    int size;
    FRAME_TYPE type;

};


#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_FRAME_H
