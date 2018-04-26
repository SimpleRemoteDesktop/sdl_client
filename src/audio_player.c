
#include "audio_player.h"
SDL_AudioSpec   wanted_spec, spec;

//https://wiki.libsdl.org/SDL_AudioSpec
//https://loka.developpez.com/tutoriel/sdl/sons/


void audioCallback(void *udata, Uint8 *stream, int len)
{
   
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"stream buffer length %d \n", len);
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"Raw audio buffer size %d \n", raw_audio_buffer->lenght);
	if(raw_audio_buffer->lenght >= len) {
       memcpy(stream, raw_audio_buffer->buffer, len);
       memcpy(raw_audio_buffer->buffer,
              raw_audio_buffer->buffer+len,
              raw_audio_buffer->lenght - len);
       raw_audio_buffer->lenght -= len;
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"Raw audio buffer size after %d \n", raw_audio_buffer->lenght);
   } else {
       memset(stream, 0, len);
   }
}


void SRD_init_audio(int sampleRate, int channels)
{

    wanted_spec.freq = sampleRate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = channels;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE; //TODO buffer size must be sync with opus decoder
    wanted_spec.callback = audioCallback;
    wanted_spec.userdata = NULL;


    if(SDL_OpenAudio(&wanted_spec, &spec) < 0) {
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        return -1;
    }

    SDL_PauseAudio(0);
}

