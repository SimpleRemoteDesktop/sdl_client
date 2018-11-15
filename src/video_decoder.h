
#ifndef SDL_CLIENT_VIDEO_DECODER_H
#define SDL_CLIENT_VIDEO_DECODER_H


// remote desktop sdl client
#include <stdbool.h>
#include<stdint.h>
#include<time.h>
#include <stdio.h>
#include "Frame.h"
#include "Image.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};

class SoftwareVideoDecoder {
public:
    SoftwareVideoDecoder(int codecWidth, int codecHeight);
    ~SoftwareVideoDecoder();
    void decode(Frame *frame, Image* image);
    void run();

private:
    int codecWidth;
    int codecHeight;
    AVFrame *pFrame;
    AVCodecContext *pCodecCtx;
    SwsContext *sws_ctx;
};

#endif
