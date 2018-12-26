//
// Created by user on 11/12/18.
//

#include "VideoManager.h"
#include "vaapi/vaapi.h"


VideoManager::VideoManager(Queue<Frame> *videoQueue, int screenWidth, int screenHeight, SDL_Window *screen) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->videoQueue = videoQueue;
    this->screen = screen;


    this->surface = new SdlVideoRenderer(this->screenWidth, this->screenHeight, this->screen);
    this->decoder = new SoftwareVideoDecoder(this->screenWidth, this->screenHeight);

}

void VideoManager::run() {
    this->isRunning = true;
    while (this->isRunning) {
        Frame frame = this->videoQueue->pop();
        //int time = SDL_GetTicks();
        AVFrame *pFrame = av_frame_alloc();
        if (this->decoder->decode(&frame, pFrame)) {
            if (this->decoder->isVaapi) {
                vaapi_queue(pFrame, this->surface->getX11Window(), this->screenWidth, this->screenHeight);
            } else {
                this->surface->update_video_surface(pFrame);
            }
        }
        //printf("Decode d²isplay time %d \n", SDL_GetTicks() - time);
        delete[] frame.data;
        av_frame_free(&pFrame);
    }
}

VideoManager::~VideoManager() {

}

