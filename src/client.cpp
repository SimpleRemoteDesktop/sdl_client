// remote desktop sdl client
#ifdef __MINGW32__
#undef main /* Prevents SDL from overriding main() */
#define SDL_MAIN_HANDLED
#endif

#include <stdio.h>
#include "network.h"
#include "input.h"
#include "video_decoder.h"
#include "video_surface.h"
#include "audio_decoder.h"
#include "audio_player.h"


//memset(inbuf+INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);


int main(int argc, char **argv) {

	// Declare display mode structure to be filled in.
	SDL_DisplayMode current;

	//sound driver only for windows

#ifdef WIN32
	putenv("SDL_AUDIODRIVER=DirectSound");
#endif



	// set log level

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    // default value
    int width = 800;
    int height = 600;
    int bandwidth = 1000000;
    int fps = 24;
    bool withRelativeMouse = false;

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "init() \n");

	char *video_definition = argv[3];


	if (argv[4] != NULL && atoi(argv[4]) > 0) // have custom bandwidth
	{
		bandwidth = atoi(argv[4]);
	}

	if (argv[4] != NULL && atoi(argv[5]) > 0) //have custom fps
	{
		fps = atoi(argv[5]);
	}

    if(argv[6] != NULL && strcmp("+relative", argv[6]) == 0) { //relative mouse mode / else absolute
        withRelativeMouse = true;
    }

    if (video_definition != NULL) {
        if (strcmp("720p", video_definition) == 0) {
            width = 1280;
            height = 720;
        } else if (strcmp("800p", video_definition) == 0) {
            width = 1280;
            height = 800;
        } else if (strcmp("1080p", video_definition) == 0) {
            width = 1920;
            height = 1080;
        }
    }

	std::string hostname(strdup(argv[1]));
	int port = atoi(argv[2]);


	// start SDL Application

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}

	int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);

	if (should_be_zero != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not get Current display resolution %s", SDL_GetError());
	} else {
		//        width = current.w;
		//        height = current.h;
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "max screen resolution, width : %d, height: %d", width, height);

	}

	/* SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
	   "parameters hostname : %s, port : %d, video resolution : %sx%s, bandwidth : %d, fps : %d\n",
	   hostname.c_str(),
	   port,
	   width,
	   height,
	   bandwidth,
	   fps);*/

    PlayerManager *player = new PlayerManager(hostname, port, width, height, bandwidth, fps, withRelativeMouse);
    player->start();

	exit(1);
}


