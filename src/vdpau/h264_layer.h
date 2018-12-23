#ifndef _H264_LAYER_H_
#define _H264_LAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vdpau_layer.h"

#define H264_INFO_HDR_LEN 4
#define H264_INFO_HDR_OFFSET (4 + 4 + 8 + 4)

typedef struct {
    VdpPictureInfoH264 info;
    int datalen;
    unsigned char *data;
}h264_frame;

h264_frame *create_h264_frame(char **buffer);

#endif
