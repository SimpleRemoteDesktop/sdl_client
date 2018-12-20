#include <SDL2/SDL_net.h>

#include "video_decoder.h"
#include "video_surface.h"

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
}

bool SoftwareVideoDecoder::decode(Frame *frame, AVFrame *image) {
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
        lenght = avcodec_decode_video2(pCodecCtx, image, &frameFinished, &packet);


        if (lenght < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while decoding frame\n");
        }
        // Did we get a video frame?
        if (frameFinished) {
            return true;
        }
        if (packet.data) {
            packet.size -= lenght;
            packet.data += lenght;
        }
        av_free_packet(&packet);
        return false;
    }
}

SoftwareVideoDecoder::~SoftwareVideoDecoder() {
    avcodec_close(pCodecCtx);
}
