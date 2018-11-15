// remote desktop sdl client
#ifdef __MINGW32__
#undef main /* Prevents SDL from overriding main() */
#endif

#include <stdio.h>
#include "client.h"
#include "config.h"
#include "network.h"
#include "input.h"
#include "video_decoder.h"
#include "video_surface.h"
#include "audio_decoder.h"
#include "audio_player.h"


//memset(inbuf+INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);

Configuration *configuration;


int main(int argc, char *argv[]) {

    // Declare display mode structure to be filled in.
    SDL_DisplayMode current;

    //sound driver only for windows

#ifdef WIN32
    putenv("SDL_AUDIODRIVER=DirectSound");
#endif


    // set log level

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    // default value workaround
    //
    configuration = (Configuration *) malloc(sizeof(Configuration));
    configuration->screen = (Screen *) malloc(sizeof(Screen));
    configuration->codec = (Codec *) malloc(sizeof(Codec));
    configuration->server = (Server *) malloc(sizeof(Server));
    configuration->maxScreenSize = (Screen *) malloc(sizeof(Screen));
    configuration->screen->width = 800;
    configuration->maxScreenSize->height = 600;
    configuration->maxScreenSize->width = 800;
    configuration->screen->height = 600;
    configuration->codec->width = 800;
    configuration->codec->height = 600;
    configuration->bandwidth = 1000000;
    configuration->fps = 25;


    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init() \n");

    configuration->server->hostname = strdup(argv[1]);
    configuration->server->port = atoi(argv[2]);
    char *video_definition = argv[3];


    if (argv[4] != NULL && atoi(argv[4]) > 0) // have custom bandwidth
    {
        configuration->bandwidth = atoi(argv[4]);
    }

    if (argv[4] != NULL && atoi(argv[5]) > 0) //have custom fps
    {
        configuration->fps = atoi(argv[5]);
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "parameters hostname : %s, port : %d, video resolution : %s, bandwidth : %d, fps : %d \n",
                configuration->server->hostname,
                configuration->server->port,
                video_definition,
                configuration->bandwidth,
                configuration->fps);

    if (video_definition != NULL) {
        if (strcmp("720p", video_definition) == 0) {
            configuration->screen->width = configuration->codec->width = 1280;
            configuration->screen->height = configuration->codec->height = 720;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "swicth video resolution to %dx%d \n",
                        configuration->codec->width, configuration->codec->height);
        }
        if (strcmp("800p", video_definition) == 0) {
            configuration->screen->width = configuration->codec->width = 1280;
            configuration->screen->height = configuration->codec->height = 800;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "swicth video resolution to %dx%d \n",
                        configuration->codec->width, configuration->codec->height);
        }

        if (strcmp("1080p", video_definition) == 0) {
            configuration->screen->width = configuration->codec->width = 1920;
            configuration->screen->height = configuration->codec->height = 1080;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "swicth video resolution to %dx%d \n",
                        configuration->codec->width, configuration->codec->height);
        }
    }


    // start SDL Application

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL - %s\n", SDL_GetError());
        SRD_exit();
    }

    int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);

    if (should_be_zero != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not get Current display resolution %s", SDL_GetError());
    } else {
        configuration->maxScreenSize->width = current.w;
        configuration->maxScreenSize->height = current.h;
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "max screen resolution, width : %d, height: %d",
                    configuration->maxScreenSize->width, configuration->maxScreenSize->height);

    }
    std::string hostname(strdup(argv[1]));
    int port = atoi(argv[2]);
    PlayerManager *player = new PlayerManager(hostname, port, 1280, 720, 1000000, 30);
    player->start();
}


void SRD_exit() {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "exiting application");
    exit(1);
}

