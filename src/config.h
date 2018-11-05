#ifndef SDL_CLIENT_CONFIG_H
#define SDL_CLIENT_CONFIG_H

extern "C" {
typedef struct Screen Screen;
struct Screen {
    int width;
    int height;
};

typedef struct Codec Codec;
struct Codec {
    int width;
    int height;
};

typedef struct Server Server;
struct Server {
    char *hostname;
    int port;
};

typedef struct Configuration Configuration;
struct Configuration {
    Screen *screen;
    Screen *maxScreenSize;
    Codec *codec;
    Server *server;
    int bandwidth;
    int fps;
};


}

#endif