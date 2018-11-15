
#include <cstring>
#include "network.h"


int Network::init_network(Queue<Frame> *video, Queue<Frame> *audio) {
    inbuf_average = 0;
    this->inbuf = new uint8_t[500000];
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Init network interface");
    if (SDLNet_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDLNet_Init: %s\n", SDLNet_GetError());
        return 1;
    }

    this->videoQueue = video;
    this->audioQueue = audio;
}

int Network::SRDNet_send_start_packet(int codecWidth, int codecHeight, int bandwidth, int fps) {
    // inital packet with information
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, " network : sending start packet");
    struct Message init;
    init.type = TYPE_ENCODER_START;
    init.x = 1;
    init.y = 1;
    init.button = 1;
    init.keycode = 1;
    init.fps = fps;
    init.codec_width = codecWidth;
    init.codec_height = codecHeight;
    init.bandwidth = bandwidth;
    init.sdl = 0;
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,
                   "sending init frame : type: %d, fps: %d, codec width: %d, codec height: %d, bandwidth: %d",
                   init.type, init.fps, init.codec_width, init.codec_height, init.bandwidth);
    SDLNet_TCP_Send(control_socket, (void *) &init, sizeof(init));
    return 0;

}

int Network::SRDNet_send_stop_packet() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, " network : sending stop packet");
    struct Message stop;
    stop.type = TYPE_ENCODER_STOP;
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "sending stop frame");
    SDLNet_TCP_Send(control_socket, (void *) &stop, sizeof(stop));
    return 0;

}

int Network::SRDNet_get_frame_type() {
    SRD_ensure(4);
    return this->SRD_readUInt32();

}

int Network::SRDNet_get_frame_length() {
    this->SRD_ensure(4);
    return SRD_readUInt32();
}


void Network::SRDNet_Empty_input_buffer() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "cleaning input tcp buffer");
    char net_in[2048];
    int i;

    do {
        i = SDLNet_TCP_Recv(control_socket, net_in, 1024);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%d bytes remains", i);
    } while (i > 0);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "cleaned");

}

void Network::SRD_ensure(int nbytes) {
    char net_in[nbytes + 1];
    do {
        int net_lenght = SDLNet_TCP_Recv(control_socket, net_in, nbytes); //FIXME : adjust max data
        if (net_lenght <= 0) {
            // TCP Connection is broken. (because of error or closure)
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Broken socket disconnected");
            SDLNet_TCP_Close(control_socket);
            //FIXME

        } else {
            memcpy(inbuf + inbuf_average, net_in, net_lenght);
            inbuf_average = net_lenght + inbuf_average;
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "reading %d bytes from network. buffer average : %d\n",
                           net_lenght, inbuf_average);
        }
    } while (inbuf_average < nbytes);

}

int Network::SRD_readUInt32() {
    uint8_t *data = this->SRD_read(4);
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, " %x %x %x %x \n", data[0], data[1], data[2], data[3]);
    uint32_t num =
            (uint32_t) data[3] << 24 |
            (uint32_t) data[2] << 16 |
            (uint32_t) data[1] << 8 |
            (uint32_t) data[0];
    return num;
}

uint8_t *Network::SRD_read(int nbytes) {
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "average byte : %d, read bytes : %d \n", this->inbuf_average, nbytes);
    uint8_t *data = new uint8_t[nbytes];
    std::memcpy(data, this->inbuf, nbytes) ;
    std::memcpy(this->inbuf, this->inbuf + nbytes, this->inbuf_average - nbytes ) ;
    inbuf_average -= nbytes;
    return data;
}

void Network::connect(std::string hostname, int port) {
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, hostname.c_str(), port) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "unable to resolve address %s , port %d\n",
                     hostname.c_str(),
                     port);
        //TODO throw error
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "connecting to server");
    if (!(control_socket = SDLNet_TCP_Open(&ip))) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());

        //TODO throw error
    }

}

int Network::send(Message *message) {
    return SDLNet_TCP_Send(control_socket, (void *) message, sizeof(Message));
}


void Network::run() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "starting network thread");
    this->isRunning = true;
    while (this->isRunning) {

        // get frame from network
        FRAME_TYPE type = (FRAME_TYPE) this->SRDNet_get_frame_type();
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "frame type %d", type);
        int length = this->SRDNet_get_frame_length();
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "frame length %d", length);

        this->SRD_ensure(length);
        Frame *frame = new Frame();
        frame->data = this->SRD_read(length);
        frame->size = length;
        frame->type = type;

        if (frame->type == VIDEO_FRAME) {
            this->videoQueue->push(*frame);
        } else if (frame->type == AUDIO_FRAME) {
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "AUDIO frame length %d", frame->size);
            this->audioQueue->push(*frame);
        } else {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknow frame type %d", frame->type);
        }
    }
}



