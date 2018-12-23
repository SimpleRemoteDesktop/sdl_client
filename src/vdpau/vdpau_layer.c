#include "vdpau_layer.h"

vdp_ctx *init_vdpau_ctx(Display *display, int screen) {
    VdpStatus status = VDP_STATUS_OK;

    if (!display) {
        fprintf(stderr,"Display cannot be null\n");
        return NULL;
    }
    vdp_ctx *ctx = malloc(sizeof(vdp_ctx));
    if (!ctx) return NULL;

    ctx->display = display;
    ctx->screen = screen;

    fprintf(stdout,"Initializing vdpau context\n");
    status = vdp_device_create_x11(ctx->display, ctx->screen, &ctx->vdp_device, 
                                   &ctx->vdp_get_proc_address);
    fprintf(stdout,"Created x11 device\n");

    ctx->table = init_vdpau_functions(ctx);

    return ctx;
}

vdp_functable *init_vdpau_functions(vdp_ctx *ctx) {
    fprintf(stdout,"Initializing vpdau functions\n");
    VdpStatus status;
    vdp_functable *table = malloc(sizeof(vdp_functable));
    if (!table) return NULL;

    void *iter;
    int i;
    int size = sizeof(vdp_functable) / sizeof(void*);
   
    for (i = 0, iter = table; i < size; i++, iter+=(sizeof(void*))) {
        status = ctx->vdp_get_proc_address(ctx->vdp_device, refs[i], iter);
        if (status != VDP_STATUS_OK) printf("Failed\n");
    }
    unsigned int version;
    const char *info;
    table->vdp_get_information_string(&info); 
    table->vdp_get_api_version(&version);
    printf("%s %d\n",info,version);
    return table;
}

vdp_decoder_ctx *init_vdpau_decoder(vdp_ctx *ctx, char **buffer) {
    VdpStatus status = VDP_STATUS_OK;
    vdp_decoder_ctx *dec_ctx = malloc(sizeof(vdp_decoder_ctx));
    if (!dec_ctx) {
        fprintf(stderr,"Failed to allocate decoder context\n");
        goto error;
    }
    if (!memcpy(&dec_ctx->width, *buffer, 4)) {
        fprintf(stderr,"Failed to copy width\n");
        goto error;
    }
    *buffer += 4;

    if (!memcpy(&dec_ctx->height, *buffer, 4)) {
        fprintf(stderr,"Failed to copy height\n");
        goto error;
    }
    *buffer +=4;

    if (!memcpy(&dec_ctx->ratio, *buffer, 8)) {
        fprintf(stderr,"Failed to copy ratio\n");
        goto error;
    }
    *buffer += 8;

     if (!memcpy(&dec_ctx->profile, *buffer, 4)) {
        fprintf(stderr,"Failed to copy height\n");
        goto error;
    }
    *buffer +=4;
    
    status = ctx->table->vdp_decoder_create(ctx->vdp_device, dec_ctx->profile, dec_ctx->width,
                                            dec_ctx->height, 16, &dec_ctx->vdp_decoder); 
    if (status != VDP_STATUS_OK) {
        fprintf(stderr,"Oeps, something went wrong\n");
        goto error;
    }
    dec_ctx->ctx = ctx;
    return dec_ctx;
error:
    if (dec_ctx) free(dec_ctx);
    return NULL;
}

VdpStatus init_vdpau_surfaces(vdp_decoder_ctx *dec_ctx) {
    int i;
    VdpStatus status = VDP_STATUS_OK;

    for (i = 0; i < NUMBER_OF_SURFACES; i++) {
        dec_ctx->surfaces[i] = VDP_INVALID_HANDLE;
        status = dec_ctx->ctx->table->vdp_video_surface_create(dec_ctx->ctx->vdp_device,
                                                         VDP_CHROMA_TYPE_420, dec_ctx->width,
                                                         dec_ctx->height, &dec_ctx->surfaces[i]);
        if (status != VDP_STATUS_OK) {
            fprintf(stderr,"Failed to create surface\n");
            return -1;
        }
    }
    fprintf(stdout,"Created surfaces\n");
    return 0;
}

VdpStatus init_vdpau_output(vdp_decoder_ctx *dctx) {
    VdpStatus status = VDP_STATUS_OK;
    vdp_functable *vft;
    vft = dctx->ctx->table;

    status = vft->vdp_output_surface_create(dctx->ctx->vdp_device, 
                                            VDP_RGBA_FORMAT_B8G8R8A8, 
                                            dctx->width, dctx->height, 
                                            &dctx->ctx->display_surface);
    if (status != VDP_STATUS_OK) {
        fprintf(stdout,"Failed to create output surface\n");
        return status;
    }

    status = vft->vdp_presentation_queue_target_create_x11(dctx->ctx->vdp_device,
                                            dctx->ctx->win, 
                                            &dctx->ctx->queue_target);
    if (status != VDP_STATUS_OK) {
        fprintf(stdout,"Failed to create queue target X11\n");
        return status;
    }  
    fprintf(stdout,"X11 Presentation queue target created\n");

    vft->vdp_presentation_queue_create(dctx->ctx->vdp_device, 
                                       dctx->ctx->queue_target,
                                       &dctx->ctx->queue);
    if (status != VDP_STATUS_OK) {
        fprintf(stderr,"Failed to create queue\n");
        return status;
    }
    fprintf(stdout,"Presentation queue created\n"); 
    return status;
}

vdp_mixer_ctx *init_vdpau_mixer(vdp_decoder_ctx *ctx) {
    VdpStatus status = VDP_STATUS_OK;
    vdp_mixer_ctx *mixer = malloc(sizeof(vdp_mixer_ctx));
    if (!mixer) {
        fprintf(stderr,"Failed to allocate mixer\n");
        return NULL;
    }
    mixer->mixer_features[0] = VDP_VIDEO_MIXER_FEATURE_NOISE_REDUCTION;
    mixer->mixer_features[1] = VDP_VIDEO_MIXER_FEATURE_SHARPNESS;
    mixer->mixer_features[2] = VDP_VIDEO_MIXER_FEATURE_DEINTERLACE_TEMPORAL;
    mixer->mixer_features[3] = VDP_VIDEO_MIXER_FEATURE_DEINTERLACE_TEMPORAL_SPATIAL;
    mixer->mixer_features[4] = VDP_VIDEO_MIXER_FEATURE_INVERSE_TELECINE;
    VdpVideoMixerParameter params[] = { VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_WIDTH, 
                                        VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_HEIGHT,
                                        VDP_VIDEO_MIXER_PARAMETER_CHROMA_TYPE, 
                                        VDP_VIDEO_MIXER_PARAMETER_LAYERS };
    mixer->chroma = VDP_CHROMA_TYPE_420;
    int num_layers = 3;
    void const *param_values[] = {&ctx->width, &ctx->height, &mixer->chroma,&num_layers};
    
    status = ctx->ctx->table->vdp_video_mixer_create(ctx->ctx->vdp_device, 5, mixer->mixer_features,
                                                     4, params, param_values, &mixer->vdp_mixer);
    if (status != VDP_STATUS_OK) {
        fprintf(stderr,"Failed to create mixer\n");
        return NULL;
    }
    fprintf(stdout,"Video mixer created\n");

    return mixer;
}
