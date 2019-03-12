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
#ifdef __linux__
    		if (this->decoder->isVaapi) {
                SDL_Rect size = this->surface->getSize();
                vaapi_queue(pFrame, this->surface->getX11Window(), size.w, size.h);
            } else {
#endif
                this->surface->update_video_surface(pFrame);
#ifdef __linux__
            }
#endif
        }
        //printf("Decode d²isplay time %d \n", SDL_GetTicks() - time);
        delete[] frame.data;
        av_frame_free(&pFrame);
    }
}

VideoManager::~VideoManager() {

}

void VideoManager::scaleSourceToDestinationSurface(SDL_Rect* src, SDL_Rect* dst)
{
    int dstH = dst->w * src->h / src->w;
    int dstW = dst->h * src->w / src->h;

    if (dstH > dst->h) {
        dst->y = 0;
        dst->x = (dst->w - dstW) / 2;
        dst->w = dstW;
        SDL_assert(dst->w * src->h / src->w <= dst->h);
    }
    else {
        dst->x = 0;
        dst->y = (dst->h - dstH) / 2;
        dst->h = dstH;
        SDL_assert(dst->h * src->w / src->h <= dst->w);
    }
}

