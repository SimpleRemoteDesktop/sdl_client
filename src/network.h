#ifndef SDL_CLIENT_NETWORK_H
#define SDL_CLIENT_NETWORK_H

#include <stdbool.h>
#include<stdint.h>
#include<time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#include <stdio.h>
#include <string>
#include "audio_decoder.h"
#include "Queue.h"
#include "Frame.h"

#include "tcpSocketWrapper.h"

enum type {
    TYPE_KEY_DOWN = 1,
    TYPE_KEY_UP = 2,
    TYPE_MOUSE_MOTION = 3,
    TYPE_MOUSE_DOWN = 4,
    TYPE_MOUSE_UP = 5,
    TYPE_ENCODER_START = 6,
    TYPE_ENCODER_STOP = 7,
    TYPE_MOUSE_RELATIVE_MOTION = 8,
    TYPE_ENCODER_TJPEG = 9,
    TYPE_MOUSE_WHEEL = 10,

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
    void run();
    int init_network(Queue<Frame> *video, Queue<Frame> *audio);
    void connect(std::string hostname, int port);
    int SRDNet_get_frame_number();
    int SRDNet_get_frame_length();
    void SRD_ensure(int nbytes);
    int SRD_readUInt32();
    uint8_t *SRD_read(int nbytes);
    int SRDNet_send_start_packet(int codecWidth, int codecHeight, int bandwidth, int fps);
    int SRDNet_send_stop_packet();
    int SRDNet_get_frame_type();
    int send(Message *message);

    Queue<Frame> *videoQueue;
    Queue<Frame> *audioQueue;
    bool isRunning;
    uint8_t *inbuf;
    int inbuf_average;
};

int network_thread(void *data);

#endif
