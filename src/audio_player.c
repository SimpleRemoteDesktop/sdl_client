
#include "audio_player.h"
SDL_AudioSpec   wanted_spec, spec;

//https://wiki.libsdl.org/SDL_AudioSpec
//https://loka.developpez.com/tutoriel/sdl/sons/


void audioCallback(void *udata, Uint8 *stream, int len)
{
   
	if(SRD_raw_audio_buffer->size >= len) {
       memcpy(stream, SRD_raw_audio_buffer->buffer, len);
       memcpy(SRD_raw_audio_buffer->buffer,
              SRD_raw_audio_buffer->buffer+len,
              SRD_raw_audio_buffer->size - len);
       SRD_raw_audio_buffer->size -= len;
   }
   if(SRD_raw_audio_buffer->size >= 6*len) {
	    //TODO refactor => cleaning
       SRD_raw_audio_buffer->size = 0;
	}
}


void SRD_init_audio(int sampleRate, int channels)
{

    wanted_spec.freq = sampleRate;
    wanted_spec.format = AUDIO_S16LSB;
    wanted_spec.channels = channels;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE; //TODO buffer size must be sync with opus decoder
    wanted_spec.callback = audioCallback;
    wanted_spec.userdata = NULL;


    audioDeviceID = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if( audioDeviceID < 0) {
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        return -1;
    }

    SDL_PauseAudioDevice( audioDeviceID, 0);
}
#ifdef __cplusplus
}
#endif
