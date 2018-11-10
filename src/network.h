#ifndef SDL_CLIENT_NETWORK_H
#define SDL_CLIENT_NETWORK_H

#include <stdbool.h>
#include<stdint.h>
#include<time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_thread.h>

#include <stdio.h>
#include <string>
#include "audio_decoder.h"
#include "Queue.h"
#include "Frame.h"


#define INBUF_SIZE 1000000
#define FF_INPUT_BUFFER_PADDING_SIZE 32


enum type {
    TYPE_KEY_DOWN = 1,
    TYPE_KEY_UP = 2,
    TYPE_MOUSE_MOTION = 3,
    TYPE_MOUSE_DOWN = 4,
    TYPE_MOUSE_UP = 5,
    TYPE_ENCODER_START = 6,
    TYPE_ENCODER_STOP = 7
};

struct Message {
    int type;
    float x;
    float y;
    int button;
    int keycode;
    int scancode;
    int codec_width;
    int codec_height;
    int bandwidth;
    int fps;
    int sdl;
};

class Network {
public:
    void SRDNet_Empty_input_buffer();
    int init_network(Queue<Frame> *video, Queue<Frame> *audio);
    void connect(std::string hostname, int port);
    int SRDNet_get_frame_number();
    int SRDNet_get_frame_length();
    void SRD_ensure(int nbytes);
    int SRD_readUInt32();
    uint8_t *SRD_read(int nbytes);
    int SRDNet_send_start_packet();
    int SRDNet_send_stop_packet();
    int SRDNet_get_frame_type();

    Queue<Frame> *videoQueue;
    Queue<Frame> *audioQueue;
    std::string hostname;
    int port;
};

int network_thread(void *data);

#endif