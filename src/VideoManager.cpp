//
// Created by user on 11/12/18.
//

#include "VideoManager.h"


VideoManager::VideoManager(Queue<Frame> *videoQueue, int screenWidth, int screenHeight, SDL_Window* screen) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->videoQueue = videoQueue;
    this->screen = screen;


    this->surface = new SdlVideoRenderer(this->screenWidth, this->screenHeight, this->screen);
    this->decoder = new SoftwareVideoDecoder(this->screenWidth, this->screenHeight);

}

void VideoManager::run() {
    this->isRunning = true;
    while(this->isRunning) {
        Frame frame = this->videoQueue->pop();
        Image *image = new Image(this->screenWidth, this->screenHeight);
        this->decoder->decode(&frame, image);
        this->surface->update_video_surface(image);
        delete [] frame.data;
        delete image;
    }
}

VideoManager::~VideoManager() {

}

