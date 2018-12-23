#include "h264_layer.h"

// Create a h264 frame
h264_frame *create_h264_frame(char **buffer) {
    h264_frame *nf = malloc(sizeof(h264_frame));
    if (!nf) {
        fprintf(stderr,"Failed to create h264 frame\n");
        return NULL;
    }

    if (!memcpy(&nf->info,*buffer,sizeof(nf->info))) {
        fprintf(stderr,"Failed to memcpy header\n");
    }

    *buffer += sizeof(nf->info); //Shift size of info struct

    if (!memcpy((char*)&nf->datalen,*buffer,4)) {
        fprintf(stderr,"Failed to memcpy header\n");
    }
    
    *buffer += 4; //shift 4 bytes from header

    nf->data = malloc(nf->datalen);
    if (!nf->data) {
        fprintf(stdout,"Failed to create data\n");
        return NULL;
    }
    if (!memcpy(nf->data, *buffer, nf->datalen)) {
        fprintf(stdout,"Failed to memcpy data\n");
        return NULL;
    }   
    *buffer += nf->datalen;

    return nf;
}

