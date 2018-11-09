
#ifndef SDL_CLIENT_VIDEO_DECODER_H
#define SDL_CLIENT_VIDEO_DECODER_H


// remote desktop sdl client
#include <stdbool.h>
#include<stdint.h>
#include<time.h>
#include <stdio.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};

int init_video_decoder(int codec_width, int codec_height);
int decode_video_frame(uint8_t *frame,int frame_length, Configuration *conf); 
void destroy_decoder();
#endif
