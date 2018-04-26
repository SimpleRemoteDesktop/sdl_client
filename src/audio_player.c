
#include "audio_player.h"



SDL_AudioSpec   wanted_spec, spec;

void init_audio()
{

wanted_spec.freq = aCodecCtx->sample_rate;
wanted_spec.format = AUDIO_S16SYS;
wanted_spec.channels = aCodecCtx->channels;
wanted_spec.silence = 0;
wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
wanted_spec.callback = audio_callback;
wanted_spec.userdata = aCodecCtx;


if(SDL_OpenAudio(&wanted_spec, &spec) < 0) {
fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
return -1;
}

SDL_PauseAudio(0);
}

