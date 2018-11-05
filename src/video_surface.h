#ifndef SDL_CLIENT_VIDEO_SURFACE_H
#define SDL_CLIENT_VIDEO_SURFACE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_thread.h>



void init_video(int screen_width, int screen_height);
void update_video_surface();
void destroy_texture();
void SRD_init_renderer_texture(int w, int h);
void SRD_UpdateScreenResolution();
#ifdef __cplusplus
}
#endif
#endif