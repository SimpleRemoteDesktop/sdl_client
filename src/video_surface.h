#ifndef SDL_CLIENT_VIDEO_SURFACE_H
#define SDL_CLIENT_VIDEO_SURFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_thread.h>
#include "video_decoder.h"
#include "network.h"


//void init_video(int screen_width, int screen_height);
//void SRD_UpdateScreenResolution();

class SdlVideoRenderer {
public:
    SdlVideoRenderer(int rendererWidth, int rendererHeight, SDL_Window *screen);
    void update_video_surface(AVFrame* image);
    void destroy_texture();

private:
    SDL_Renderer *renderer;
    int width;
    int height;
    SDL_Texture *bmp;
    SDL_Window *screen;
};

#endif