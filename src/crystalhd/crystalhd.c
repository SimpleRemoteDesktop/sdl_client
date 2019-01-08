#ifdef __cplusplus
extern "C" {
#endif
#include <libavutil/pixfmt.h>
#include "crystalhd.h"

static int hw_decoder_init(AVCodecContext *ctx, const enum AVHWDeviceType type) {
    int err = 0;
    static AVBufferRef *hw_device_ctx = NULL;
    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type, NULL, NULL, 0)) < 0) {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

    return err;
}

static enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts) {
    const enum AVPixelFormat *p;

    for (p = pix_fmts; *p != -1; p++) {
        if (*p == AV_PIX_FMT_YUYV422)
            return *p;
    }

    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

int crystalhd_init(AVCodecContext *pCodecCtx) {
    enum AVHWDeviceType type = av_hwdevice_find_type_by_name("h264_crystalhd");
    pCodecCtx->get_format = get_hw_format;
    hw_decoder_init(pCodecCtx, type);
    return 0;

}
#ifdef __cplusplus
};
#endif
