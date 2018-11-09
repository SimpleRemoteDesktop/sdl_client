//
// Created by sylvain on 18/04/18.
//


#ifndef SDL_CLIENT_AUDIO_DECODER_H
#define SDL_CLIENT_AUDIO_DECODER_H


#include <SDL2/SDL.h>
#include <stdlib.h>

typedef struct {
    unsigned char buffer[1000000];
    unsigned int size;
} SRD_Raw_Audio_Buffer;


void SRD_audio_decoder_init(int sampleRate, int channels);
void SRD_audio_decode(unsigned char *audioFrame, int size);



#endif //SDL_CLIENT_AUDIO_DECODER_H
