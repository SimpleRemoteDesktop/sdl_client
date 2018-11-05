
#ifndef SDL_CLIENT_H
#define SDL_CLIENT_H

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_thread.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "config.h"



int video_thread(void* data); 
void SRD_init_renderer(Configuration* configuration);
void SRD_close();
void SRD_start_video();
void SRD_exit();
}

#endif