#include <SDL2/SDL_net.h>

#include "video_decoder.h"
#include "video_surface.h"

// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55, 28, 1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif


SoftwareVideoDecoder::SoftwareVideoDecoder(int codecWidth, int codecHeight) {
    this->codecWidth = codecWidth;
    this->codecHeight = codecHeight;

    AVCodec *pCodec = NULL;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "starting with codec resolution %dx%d", codecWidth, codecHeight);
    av_register_all();
    avformat_network_init(); //FIXME alway use ?

    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (pCodec == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec!\n");
        //FIXME : thorw error
    }

    // Copy context
    this->pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->width = codecWidth; //TODO set value
    pCodecCtx->height = codecHeight; // TODO set value
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P; // TODO must support multiple value

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        // FIXME throw error
    }



    // initialize SWS context for software scaling
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "initialize SWS context for software scaling\n ");
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "input w: %d h: %d", pCodecCtx->width, pCodecCtx->height);
    sws_ctx = sws_getContext(pCodecCtx->width,
                             pCodecCtx->height,
                             pCodecCtx->pix_fmt,
                             pCodecCtx->width,
                             pCodecCtx->height,
                             AV_PIX_FMT_YUV420P,
                             SWS_FAST_BILINEAR,
                             NULL,
                             NULL,
                             NULL
    ); //TODO is used ?




}

void SoftwareVideoDecoder::decode(Frame *frame, Image *image) {
    // Allocate video frame
    pFrame = av_frame_alloc();

    AVPacket packet;
    // received data loop
    packet.data = NULL;
    packet.size = 0;
    av_init_packet(&packet);

    packet.data = frame->data;
    packet.size = frame->size;

    while (packet.size > 0) {

        int lenght;
        // Decode video frame
        int frameFinished;
        lenght = avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);


        if (lenght < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while decoding frame\n");
        }
        // Did we get a video frame?
        if (frameFinished) {
            AVPicture pict;
            pict.data[0] = image->yPlane;
            pict.data[1] = image->uPlane;
            pict.data[2] = image->vPlane;
            pict.linesize[0] = image->width;
            pict.linesize[1] = image->uvPitch;
            pict.linesize[2] = image->uvPitch;

            // Convert the image into YUV format that SDL uses
            sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                      pFrame->linesize, 0, pCodecCtx->height,
                      pict.data, pict.linesize);

        }
        if (packet.data) {
            packet.size -= lenght;
            packet.data += lenght;
        }
        av_free_packet(&packet);
    }
}

SoftwareVideoDecoder::~SoftwareVideoDecoder() {
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
}
