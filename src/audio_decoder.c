//
// Created by sylvain on 18/04/18.
//

#include "audio_decoder.h"
#include <opus/opus.h>
#include <stdio.h>
# define MAX_FRAME_SIZE 6*960 // FIXME 6 * encoder frame size ?
int err;
OpusDecoder *decoder;

void SRD_audio_decoder_init(int sampleRate, int channels)
{

    raw_audio_buffer = (Raw_Audio_Buffer*) malloc(sizeof(Raw_Audio_Buffer));
    raw_audio_buffer->lenght = 0;


    decoder = opus_decoder_create(sampleRate, channels, &err);
    if (err<0)
    {
        fprintf(stderr, "failed to create decoder: %s\n", opus_strerror(err));
        return exit(1); //FIXME
    }
}


void SRD_audio_decode(unsigned char* audioFrame, int size)
{
    opus_int16* output_audio_raw = (opus_int16*) malloc(3840);
    memset(output_audio_raw, 0, 3840);
    int frame_size = opus_decode(decoder, (const unsigned char*) audioFrame, size, output_audio_raw, 480, 0);
    if (frame_size<0)
    {
        fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
        exit(1); //FIXME
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "opus decoded frame_size : %d", frame_size);
    int buffer_length = frame_size * 2 * sizeof(opus_int16); //FIXME  channels numbers
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"SDL_LockAudio writting %d  bytes \n ", buffer_length);
    SDL_LockAudioDevice(audioDeviceID);
    memcpy(raw_audio_buffer->buffer+raw_audio_buffer->lenght, output_audio_raw,buffer_length);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"[ OPUS DECODER : ] current buffer length %d , add %d\n", raw_audio_buffer->lenght, buffer_length);
    raw_audio_buffer->lenght += buffer_length;
    SDL_UnlockAudioDevice(audioDeviceID);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"SDL_Unlock_audio \n");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"[OPUS_DECODER : ] raw buffer size %d \n", raw_audio_buffer->lenght);

}
