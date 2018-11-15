//
// Created by user on 11/12/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_IMAGE_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_IMAGE_H

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_log.h>

class Image {
public:
    Image(int width, int height);

    int width;
    int height;
    int yPlaneSz;
    int uvPlaneSz;
    Uint8 *yPlane;
    Uint8 *uPlane;
    Uint8 *vPlane;
    int uvPitch;
};


#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_IMAGE_H
