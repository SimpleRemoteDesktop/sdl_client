//
// Created by user on 11/12/18.
//

#include "PlayerManager.h"
#include "input.h"

int video_thread_fn(void *data) {
    VideoManager *vm = (VideoManager *) data;
    vm->run();
}

int audio_thread_fn(void *data) {
    AudioManager *am = (AudioManager *) data;
    am->run();
}

int network_thread_fn(void *data) {
    Network *net = (Network *) data;
    net->run();
}

int input_thread_fn(void *data) {
    InputHandler *input = (InputHandler *) data;
    input->run();
}


PlayerManager::PlayerManager(std::string hostname, int port, int codecWidth, int codecHeight, int bandwidth, int fps, bool withRelativeMouse) {
    this->hostname = hostname;
    this->port = port;
    this->codecWidth = codecWidth;
    this->codecHeight = codecHeight;
    this->bandwidth = bandwidth;
    this->fps = fps;

    this->videoQueue = new Queue<Frame>();
    this->audioQueue = new Queue<Frame>();

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, " Creating SDL windows size %dx%d", this->codecWidth, this->codecHeight);
    this->screen = SDL_CreateWindow(
            "StreamMyDesktop Client",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            this->codecWidth,
            this->codecHeight,
            0);


    this->network = new Network();
    this->network->init_network(this->videoQueue, this->audioQueue);


    this->videoMgr = new VideoManager(this->videoQueue, this->codecWidth, this->codecHeight, this->screen);

    this->audioMgr = new AudioManager(this->audioQueue, 48000, 2);
    if(withRelativeMouse) {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    this->input = new InputHandler(this->network, this, withRelativeMouse);
}

PlayerManager::~PlayerManager() {
    //TODO
}

void PlayerManager::fullScreenHandler() {
	switch(this->screenDimension) {
		case SRD_WINDOW:
			SDL_SetWindowFullscreen(screen, SDL_WINDOW_FULLSCREEN_DESKTOP);
			this->screenDimension = SRD_FULLSCREEN_WINDOW;
			break;
		case SRD_FULLSCREEN:
			SDL_SetWindowFullscreen(screen, SDL_WINDOW_FULLSCREEN_DESKTOP);
			this->screenDimension = SRD_FULLSCREEN_WINDOW;
			break;
		case SRD_FULLSCREEN_WINDOW:
			SDL_SetWindowFullscreen(screen, 0);
			this->screenDimension = SRD_WINDOW;
			break;
	}
}

void PlayerManager::getScreenSize(int *w, int *h) {
    SDL_GetWindowSize(screen, w, h);
}

void PlayerManager::start() {

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating video thread");
    this->videoThread = SDL_CreateThread(video_thread_fn, "video_thread", (void *) this->videoMgr);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating audio thread");
    this->audioThread = SDL_CreateThread(audio_thread_fn, "audio_thread", (void *) this->audioMgr);
    this->network->connect(this->hostname, this->port);
    this->network->SRDNet_send_start_packet(this->codecWidth, this->codecHeight, this->bandwidth, this->fps);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating network thread");
    this->networkThread = SDL_CreateThread(network_thread_fn, "network_thread", this->network);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating input thread");
    this->inputThread = SDL_CreateThread(input_thread_fn, "input_thread", this->input);
    int threadReturnValue;
    SDL_WaitThread(this->videoThread, &threadReturnValue);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "thread finish exiting application");


}

void PlayerManager::quit() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Exiting application");
    exit(1);
}
