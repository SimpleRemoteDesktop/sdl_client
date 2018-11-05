//
// Created by sylvain on 18/04/18.
//

#ifndef SDL_CLIENT_AUDIO_PLAYER_H
#define SDL_CLIENT_AUDIO_PLAYER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include "audio_decoder.h"
#define SDL_AUDIO_BUFFER_SIZE 960*2


void SRD_init_audio(int sampleRate, int channels);

#endif //SDL_CLIENT_AUDIO_PLAYER_H
