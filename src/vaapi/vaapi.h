//
// Created by user on 12/26/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_VAAPI_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_VAAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <va/va.h>
#include <X11/Xlib.h>
#include <libavcodec/avcodec.h>

int vaapi_init_lib();
int vaapi_init(AVCodecContext *decoder_ctx);
void vaapi_queue(AVFrame *dec_frame, Window win, int width, int height);

#ifdef __cplusplus
};
#endif

#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_VAAPI_H
