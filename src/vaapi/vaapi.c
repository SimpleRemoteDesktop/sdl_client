#ifdef __cplusplus
extern "C" {
#endif

#include "vaapi.h"
#include <va/va.h>
#include <va/va_x11.h>
#include <libavcodec/avcodec.h>
#include <libavutil/hwcontext.h>
#include <libavutil/hwcontext_vaapi.h>
#include <X11/Xlib.h>

#define MAX_SURFACES 16

static AVBufferRef *device_ref;

static enum AVPixelFormat va_get_format(AVCodecContext *context, const enum AVPixelFormat *pixel_format) {
    AVBufferRef *hw_ctx = av_hwframe_ctx_alloc(device_ref);
    if (hw_ctx == NULL) {
        fprintf(stderr, "Failed to initialize Vaapi buffer\n");
        return AV_PIX_FMT_NONE;
    }

    AVHWFramesContext *fr_ctx = (AVHWFramesContext *) hw_ctx->data;
    fr_ctx->format = AV_PIX_FMT_VAAPI;
    fr_ctx->sw_format = AV_PIX_FMT_NV12;
    fr_ctx->width = context->coded_width;
    fr_ctx->height = context->coded_height;
    fr_ctx->initial_pool_size = MAX_SURFACES + 1;

    if (av_hwframe_ctx_init(hw_ctx) < 0) {
        fprintf(stderr, "Failed to initialize VAAPI frame context");
        return AV_PIX_FMT_NONE;
    }

    context->pix_fmt = AV_PIX_FMT_VAAPI;
    context->hw_device_ctx = device_ref;
    context->hw_frames_ctx = hw_ctx;
    context->slice_flags = SLICE_FLAG_CODED_ORDER | SLICE_FLAG_ALLOW_FIELD;
    return AV_PIX_FMT_VAAPI;
}

static int va_get_buffer(AVCodecContext *context, AVFrame *frame, int flags) {
    return av_hwframe_get_buffer(context->hw_frames_ctx, frame, 0);
}

int vaapi_init_lib() {
    return av_hwdevice_ctx_create(&device_ref, AV_HWDEVICE_TYPE_VAAPI, ":0", NULL, 0); //fixme display number
}

int vaapi_init(AVCodecContext *decoder_ctx) {
    decoder_ctx->get_format = va_get_format;
    decoder_ctx->get_buffer2 = va_get_buffer;
}

void vaapi_queue(AVFrame *dec_frame, Window win, int width, int height) {
    VASurfaceID surface = (VASurfaceID) (uintptr_t) dec_frame->data[3];
    AVHWDeviceContext *device = (AVHWDeviceContext *) device_ref->data;
    AVVAAPIDeviceContext *va_ctx = device->hwctx;
    vaPutSurface(va_ctx->display, surface, win, 0, 0, dec_frame->width, dec_frame->height, 0, 0, width, height, NULL, 0,
                 0);
}

#ifdef __cplusplus
};
#endif
