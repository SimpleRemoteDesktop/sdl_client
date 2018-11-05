//
// Created by sylvain on 18/04/18.
//
extern "C" {

#include "audio_decoder.h"
#include <opus/opus.h>
#include <stdio.h>
# define MAX_FRAME_SIZE 6*960 // FIXME 6 * encoder frame size ?
int err;
OpusDecoder *decoder;

SRD_Raw_Audio_Buffer *SRD_raw_audio_buffer;
extern SDL_AudioDeviceID audioDeviceID;

void SRD_audio_decoder_init(int sampleRate, int channels) {

    SRD_raw_audio_buffer = (SRD_Raw_Audio_Buffer *) malloc(sizeof(SRD_Raw_Audio_Buffer));
    SRD_raw_audio_buffer->size = 0;


    decoder = opus_decoder_create(sampleRate, channels, &err);
    if (err < 0) {
        fprintf(stderr, "failed to create decoder: %s\n", opus_strerror(err));
        return exit(1); //FIXME
    }
}


void SRD_audio_decode(unsigned char *audioFrame, int size) {

    opus_int16 *output_audio_raw = (opus_int16 *) malloc(MAX_FRAME_SIZE * 2 * 2);
    memset(output_audio_raw, 0, MAX_FRAME_SIZE * 2 * 2);
    int frame_size = opus_decode(decoder, (const unsigned char *) audioFrame, size, output_audio_raw, MAX_FRAME_SIZE,
                                 0);
    if (frame_size < 0) {
        fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
        exit(1); //FIXME
    }
    unsigned int buffer_length = frame_size * 2 * sizeof(opus_int16); //FIXME  channels numbers
    SDL_LockAudioDevice(audioDeviceID);
    memcpy(SRD_raw_audio_buffer->buffer + SRD_raw_audio_buffer->size, output_audio_raw, buffer_length);
    SRD_raw_audio_buffer->size = (size_t) SRD_raw_audio_buffer->size + (size_t) buffer_length;
    SDL_UnlockAudioDevice(audioDeviceID);
    //SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"[OPUS_DECODER : ] raw buffer size %u \n", SRD_raw_audio_buffer->size);

}
}
