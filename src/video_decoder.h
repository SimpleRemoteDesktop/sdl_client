
#ifndef SDL_CLIENT_VIDEO_DECODER_H
#define SDL_CLIENT_VIDEO_DECODER_H

#ifdef __cplusplus
extern "C" {
#endif
// remote desktop sdl client
#include <stdbool.h>
#include<stdint.h>
#include<time.h>
#include <stdio.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>


int init_video_decoder(int codec_width, int codec_height);
int decode_video_frame(uint8_t *frame,int frame_length, Configuration *conf); 
void destroy_decoder();
#ifdef __cplusplus
}
#endif
#endif
