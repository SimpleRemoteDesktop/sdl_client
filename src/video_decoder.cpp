#include "video_decoder.h"
#include "video_surface.h"

#ifdef HAVE_VAAPI

#include "vaapi/vaapi.h"

#endif


SoftwareVideoDecoder::SoftwareVideoDecoder(int codecWidth, int codecHeight) {
    this->codecWidth = codecWidth;
    this->codecHeight = codecHeight;
    this->isHardwareDecoder = false;

    AVCodec *pCodec = NULL;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "starting with codec resolution %dx%d", codecWidth, codecHeight);
    av_register_all();
    pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

    // Find the decoder for the video stream
    if (pCodec == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec!\n");
        //FIXME : thorw error
    }

    // Copy context
    this->pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->width = codecWidth; //TODO set value
    pCodecCtx->height = codecHeight; // TODO set value
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P; // TODO must support multiple value
    pCodecCtx->flags |= AV_CODEC_FLAG_LOW_DELAY;
    pCodecCtx->flags |= AV_CODEC_FLAG_OUTPUT_CORRUPT;
    pCodecCtx->flags |= AV_CODEC_FLAG2_SHOW_ALL;
    pCodecCtx->skip_loop_filter = AVDISCARD_ALL;

#ifdef HAVE_VAAPI
    if (vaapi_init_lib() != 0) { //FIXME should be change
        printf("err while opening VAAPI decoder\n");
    } else {
        vaapi_init(pCodecCtx);
        printf(" VAAPI decoder opened\n");
        this->isVaapi = true;
        this->isHardwareDecoder = true;
    };
#endif

    if (this->isHardwareDecoder) { //hardware decoder
        pCodecCtx->thread_count = 2;
        printf("hw accel :creating 1 thread \n");

    } else {  // SOftware decoder
        pCodecCtx->thread_type = FF_THREAD_SLICE;
        int nprocs = SDL_min(4, SDL_GetCPUCount());
        pCodecCtx->thread_count = nprocs;
        printf("software decode nbproces : %d \n", nprocs);


    }
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

        int length;
        // Decode video frame
        int frameFinished;
        length = avcodec_decode_video2(pCodecCtx, image, &frameFinished, &packet); //FIXME deprecated


        if (length < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error while decoding frame\n");
        }
        // Did we get a video frame?
        if (frameFinished) {
            return true;
        }
        if (packet.data) {
            packet.size -= length;
            packet.data += length;
        }
        av_free_packet(&packet);
        return false;
    }
}

SoftwareVideoDecoder::~SoftwareVideoDecoder() {
    avcodec_close(pCodecCtx);
}



