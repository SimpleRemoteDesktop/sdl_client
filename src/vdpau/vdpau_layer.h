#ifndef _VDPAU_LAYER_H_
#define _VDPAU_LAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <vdpau/vdpau_x11.h>

#include "h264_layer.h"

#define NUMBER_OF_SURFACES 22

typedef struct {
    VdpGetErrorString *vdp_get_error_string;
    VdpGetApiVersion *vdp_get_api_version;
    VdpGetInformationString *vdp_get_information_string;
    VdpDeviceDestroy *vdp_device_destroy;
    VdpGenerateCSCMatrix *vdp_generate_csc_matrix;
    VdpVideoSurfaceQueryCapabilities *vdp_video_surface_query_capabilities;
    VdpVideoSurfaceQueryGetPutBitsYCbCrCapabilities *vdp_video_surface_query_get_put_bits_y_cb_cr_capabilities;
    VdpVideoSurfaceCreate *vdp_video_surface_create;
    VdpVideoSurfaceDestroy *vdp_video_surface_destroy;
    VdpVideoSurfaceGetParameters *vdp_video_surface_get_parameters;
    VdpVideoSurfaceGetBitsYCbCr	*vdp_video_surface_get_bits_y_cb_cr;
    VdpVideoSurfacePutBitsYCbCr	*vdp_video_surface_put_bits_y_cb_cr;
    VdpOutputSurfaceQueryCapabilities *vdp_output_surface_query_capabilities;
    VdpOutputSurfaceQueryGetPutBitsNativeCapabilities *vdp_output_surface_query_get_put_bits_native_capabilities;
    VdpOutputSurfaceQueryPutBitsIndexedCapabilities *vdp_output_surface_query_put_bits_indexed_capabilities;
    VdpOutputSurfaceQueryPutBitsYCbCrCapabilities *vdp_output_surface_query_put_bits_y_cb_cr_capabilities;
    VdpOutputSurfaceCreate *vdp_output_surface_create;
    VdpOutputSurfaceDestroy *vdp_output_surface_destroy;
    VdpOutputSurfaceGetParameters *vdp_output_surface_get_parameters;
    VdpOutputSurfaceGetBitsNative *vdp_output_surface_get_bits_native;
    VdpOutputSurfacePutBitsNative *vdp_output_surface_put_bits_native;
    VdpOutputSurfacePutBitsIndexed *vdp_output_surface_put_bits_indexed;
    VdpOutputSurfacePutBitsYCbCr *vdp_output_surface_put_bits_y_cb_cr;
    VdpBitmapSurfaceQueryCapabilities *vdp_bitmap_surface_query_capabilities;
    VdpBitmapSurfaceCreate *vdp_bitmap_surface_create;
    VdpBitmapSurfaceDestroy *vdp_bitmap_surface_destroy;
    VdpBitmapSurfaceGetParameters *vdp_bitmap_surface_get_parameters;
    VdpBitmapSurfacePutBitsNative *vdp_bitmap_surface_put_bits_native;
    VdpOutputSurfaceRenderOutputSurface *vdp_output_surface_render_output_surface;
    VdpOutputSurfaceRenderBitmapSurface *vdp_output_surface_render_bitmap_surface;
    VdpDecoderQueryCapabilities *vdp_decoder_query_capabilities;
    VdpDecoderCreate *vdp_decoder_create;
    VdpDecoderDestroy *vdp_decoder_destroy;
    VdpDecoderGetParameters *vdp_decoder_get_parameters;
    VdpDecoderRender *vdp_decoder_render;
    VdpVideoMixerQueryFeatureSupport *vdp_video_mixer_query_feature_support;
    VdpVideoMixerQueryParameterSupport *vdp_video_mixer_query_parameter_support;
    VdpVideoMixerQueryAttributeSupport *vdp_video_mixer_query_attribute_support;
    VdpVideoMixerQueryParameterValueRange *vdp_video_mixer_query_parameter_value_range;
    VdpVideoMixerQueryAttributeValueRange *vdp_video_mixer_query_attribute_value_range;
    VdpVideoMixerCreate	*vdp_video_mixer_create;
    VdpVideoMixerSetFeatureEnables *vdp_video_mixer_set_feature_enables;
    VdpVideoMixerGetAttributeValues *vdp_video_mixer_set_attribute_values;
    VdpVideoMixerGetFeatureSupport *vdp_video_mixer_get_feature_support;
    VdpVideoMixerGetFeatureEnables *vdp_video_mixer_get_feature_enables;
    VdpVideoMixerGetParameterValues *vdp_video_mixer_get_parameter_values;
    VdpVideoMixerGetAttributeValues *vdp_video_mixer_get_attribute_values;
    VdpVideoMixerDestroy *vdp_video_mixer_destroy;
    VdpVideoMixerRender	*vdp_video_mixer_render;
    VdpPresentationQueueTargetCreateX11	*vdp_presentation_queue_target_create_x11;
    VdpPresentationQueueTargetDestroy *vdp_presentation_queue_target_destroy;
    VdpPresentationQueueCreate *vdp_presentation_queue_create;
    VdpPresentationQueueDestroy	*vdp_presentation_queue_destroy;
    VdpPresentationQueueSetBackgroundColor *vdp_presentation_queue_set_background_color;
    VdpPresentationQueueGetBackgroundColor *vdp_presentation_queue_get_background_color;
    VdpPresentationQueueGetTime	*vdp_presentation_queue_get_time;
    VdpPresentationQueueDisplay	*vdp_presentation_queue_display;
    VdpPresentationQueueBlockUntilSurfaceIdle *vdp_presentation_queue_block_until_surface_idle;
    VdpPresentationQueueQuerySurfaceStatus *vdp_presentation_queue_query_surface_status;
    VdpPreemptionCallbackRegister *vdp_preemption_callback_register;
}vdp_functable;

static int refs[] = {
    VDP_FUNC_ID_GET_ERROR_STRING,
    VDP_FUNC_ID_GET_API_VERSION,
    VDP_FUNC_ID_GET_INFORMATION_STRING,
    VDP_FUNC_ID_DEVICE_DESTROY,
    VDP_FUNC_ID_GENERATE_CSC_MATRIX,
    VDP_FUNC_ID_VIDEO_SURFACE_QUERY_CAPABILITIES,
    VDP_FUNC_ID_VIDEO_SURFACE_QUERY_GET_PUT_BITS_Y_CB_CR_CAPABILITIES,
    VDP_FUNC_ID_VIDEO_SURFACE_CREATE,
    VDP_FUNC_ID_VIDEO_SURFACE_DESTROY,
    VDP_FUNC_ID_VIDEO_SURFACE_GET_PARAMETERS,
    VDP_FUNC_ID_VIDEO_SURFACE_GET_BITS_Y_CB_CR,
    VDP_FUNC_ID_VIDEO_SURFACE_PUT_BITS_Y_CB_CR,
    VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_CAPABILITIES,
    VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_GET_PUT_BITS_NATIVE_CAPABILITIES,
    VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_PUT_BITS_INDEXED_CAPABILITIES,
    VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_PUT_BITS_Y_CB_CR_CAPABILITIES,
    VDP_FUNC_ID_OUTPUT_SURFACE_CREATE,
    VDP_FUNC_ID_OUTPUT_SURFACE_DESTROY,
    VDP_FUNC_ID_OUTPUT_SURFACE_GET_PARAMETERS,
    VDP_FUNC_ID_OUTPUT_SURFACE_GET_BITS_NATIVE,
    VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_NATIVE,
    VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_INDEXED,
    VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_Y_CB_CR,
    VDP_FUNC_ID_BITMAP_SURFACE_QUERY_CAPABILITIES,
    VDP_FUNC_ID_BITMAP_SURFACE_CREATE,
    VDP_FUNC_ID_BITMAP_SURFACE_DESTROY,
    VDP_FUNC_ID_BITMAP_SURFACE_GET_PARAMETERS,
    VDP_FUNC_ID_BITMAP_SURFACE_PUT_BITS_NATIVE,
    VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_OUTPUT_SURFACE,
    VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_BITMAP_SURFACE,
    VDP_FUNC_ID_DECODER_QUERY_CAPABILITIES,
    VDP_FUNC_ID_DECODER_CREATE,
    VDP_FUNC_ID_DECODER_DESTROY,
    VDP_FUNC_ID_DECODER_GET_PARAMETERS,
    VDP_FUNC_ID_DECODER_RENDER,
    VDP_FUNC_ID_VIDEO_MIXER_QUERY_FEATURE_SUPPORT,
    VDP_FUNC_ID_VIDEO_MIXER_QUERY_PARAMETER_SUPPORT,
    VDP_FUNC_ID_VIDEO_MIXER_QUERY_ATTRIBUTE_SUPPORT,
    VDP_FUNC_ID_VIDEO_MIXER_QUERY_PARAMETER_VALUE_RANGE,
    VDP_FUNC_ID_VIDEO_MIXER_QUERY_ATTRIBUTE_VALUE_RANGE,
    VDP_FUNC_ID_VIDEO_MIXER_CREATE,
    VDP_FUNC_ID_VIDEO_MIXER_SET_FEATURE_ENABLES,
    VDP_FUNC_ID_VIDEO_MIXER_SET_ATTRIBUTE_VALUES,
    VDP_FUNC_ID_VIDEO_MIXER_GET_FEATURE_SUPPORT,
    VDP_FUNC_ID_VIDEO_MIXER_GET_FEATURE_ENABLES,
    VDP_FUNC_ID_VIDEO_MIXER_GET_PARAMETER_VALUES,
    VDP_FUNC_ID_VIDEO_MIXER_GET_ATTRIBUTE_VALUES,
    VDP_FUNC_ID_VIDEO_MIXER_DESTROY,
    VDP_FUNC_ID_VIDEO_MIXER_RENDER,
    VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_CREATE_X11,
    VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_DESTROY,
    VDP_FUNC_ID_PRESENTATION_QUEUE_CREATE,
    VDP_FUNC_ID_PRESENTATION_QUEUE_DESTROY,
    VDP_FUNC_ID_PRESENTATION_QUEUE_SET_BACKGROUND_COLOR,
    VDP_FUNC_ID_PRESENTATION_QUEUE_GET_BACKGROUND_COLOR,
    VDP_FUNC_ID_PRESENTATION_QUEUE_GET_TIME,
    VDP_FUNC_ID_PRESENTATION_QUEUE_DISPLAY,
    VDP_FUNC_ID_PRESENTATION_QUEUE_BLOCK_UNTIL_SURFACE_IDLE,
    VDP_FUNC_ID_PRESENTATION_QUEUE_QUERY_SURFACE_STATUS,
    VDP_FUNC_ID_PREEMPTION_CALLBACK_REGISTER
};

typedef struct {
    Display *display;
    int screen;
    Window win;
    GC gc;
    VdpDevice vdp_device;
    VdpGetProcAddress *vdp_get_proc_address;
    vdp_functable *table;
    VdpOutputSurface display_surface;
    VdpPresentationQueueTarget queue_target;
    VdpPresentationQueue queue;
} vdp_ctx;

typedef struct {
    uint32_t width, height;
    double ratio;
    vdp_ctx *ctx;
    VdpDecoder vdp_decoder;
    VdpDecoderProfile profile;
    VdpVideoSurface surfaces[NUMBER_OF_SURFACES];
    int refframes[25];
} vdp_decoder_ctx;

typedef struct {
    VdpVideoMixer vdp_mixer;
    VdpVideoMixerFeature mixer_features[6];
    VdpChromaType chroma;
} vdp_mixer_ctx;

//Initialise VDPAU backend
vdp_ctx *init_vdpau_ctx(Display *disp, int screen);

//Initialise VDPAU functions
vdp_functable *init_vdpau_functions(vdp_ctx *ctx);

//Initialise VDPAU decoder
vdp_decoder_ctx *init_vdpau_decoder(vdp_ctx *ctx, char **buffer);

//Initialise VPDAU mixer
vdp_mixer_ctx *init_vdpau_mixer(vdp_decoder_ctx *ctx);

//Initialise VDPAU surfaces
VdpStatus init_vdpau_surfaces(vdp_decoder_ctx *dec_ctx);

VdpStatus init_vdpau_output(vdp_decoder_ctx *dec);

#endif
