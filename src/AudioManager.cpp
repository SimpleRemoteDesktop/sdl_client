//
// Created by user on 11/13/18.
//

#include "AudioManager.h"
#include "audio_decoder.h"
#include "audio_player.h"

AudioManager::AudioManager(Queue<Frame> *audio, int frameRate, int channels) {
    this->queue = audio;
    this->framerate = frameRate;
    this->channels = channels;

    SRD_audio_decoder_init(this->framerate, this->channels);
    SRD_init_audio(this->framerate, this->channels);


}

void AudioManager::run() {

    this->isRunning = true;
    while (this->isRunning) {
        Frame frame = this->queue->pop();
        Frame *fptr = &frame;
        SRD_audio_decode(fptr->data, fptr->size);
    }
}
