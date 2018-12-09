#ifndef SDL_CLIENT_INPUT_H
#define SDL_CLIENT_INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include<stdint.h>
#include<time.h>
#include <stdio.h>
#include "network.h"
#include "PlayerManager.h"


class InputHandler {
public:
    InputHandler(Network *network, PlayerManager* playerManager, bool withRelativeMouse);
    void run();
private:
    Network *network;
    bool ctrl_press = false;
    bool alt_press = false;
    PlayerManager *appManager;
    bool isRunning;
    bool withRelativeMouse = false;
};

#endif