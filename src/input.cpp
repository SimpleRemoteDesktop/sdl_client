#include "input.h"
#include "network.h"
#include "video_surface.h"

extern Network *network;

InputHandler::InputHandler(Network *network, PlayerManager *appManager, bool withRelativeMouse) {
    this->network = network;
    this->appManager = appManager;
    this->withRelativeMouse = withRelativeMouse;

}

void InputHandler::run() {
    this->isRunning = true;
    while (this->isRunning) {
        SDL_Event userEvent;
        if (SDL_WaitEvent(&userEvent)) {
            SDL_Delay(10);
            struct Message send;

            switch (userEvent.type) {
                case SDL_QUIT:
                    this->appManager->quit();
                    break;

                case SDL_KEYDOWN:
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "pressed key %d\n", userEvent.key.keysym.sym);
                    send.type = TYPE_KEY_DOWN;
                    if (userEvent.key.keysym.sym == 1073742048) {
                        ctrl_press = true;
                    }
                    if (userEvent.key.keysym.sym == 1073742050) {
                        alt_press = true;
                    }
                    if (userEvent.key.keysym.sym == 113 && ctrl_press && alt_press) {
                        this->appManager->quit();
                    } else if (userEvent.key.keysym.sym == 102 && ctrl_press && alt_press) {
                        this->appManager->fullScreenHandler();
                    } else {

                        send.keycode = userEvent.key.keysym.sym;
                        send.scancode = userEvent.key.keysym.scancode;
                        network->send(&send);
                    }


                    break;

                case SDL_KEYUP:
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "released key %d\n", userEvent.key.keysym.sym);
                    send.type = TYPE_KEY_UP;
                    if (userEvent.key.keysym.sym == 1073742048) {
                        ctrl_press = false;
                    }
                    if (userEvent.key.keysym.sym == 1073742050) {
                        alt_press = false;
                    }

                    send.keycode = userEvent.key.keysym.sym;
                    send.scancode = userEvent.key.keysym.scancode;
                    network->send(&send);
                    break;

                case SDL_MOUSEMOTION:
                    SDL_LogDebug(SDL_LOG_PRIORITY_VERBOSE, "mouse position x: %d, y: %d \n", userEvent.motion.x, userEvent.motion.y);
                    int w, h;
                    if(this->withRelativeMouse) {
                        send.type = TYPE_MOUSE_RELATIVE_MOTION;
                        send.x = (float) userEvent.motion.xrel;
                        send.y = (float) userEvent.motion.yrel;                       
                    } else {
                        send.type = TYPE_MOUSE_MOTION;
                        this->appManager->getScreenSize(&w, &h);
                        send.x = ((float) userEvent.motion.x / (float) w);
                        send.y = ((float) userEvent.motion.y / (float) h);
                    }
                    network->send(&send);
                    break;
                case SDL_MOUSEBUTTONDOWN: {
                    send.type = TYPE_MOUSE_DOWN;
                    switch (userEvent.button.button) {
                        case SDL_BUTTON_LEFT: {
                            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "left click down\n");
                            send.button = 1;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_RIGHT: {
                            //printf("right click down\n");
                            send.button = 2;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_MIDDLE: {
                            //printf("middle click down\n");
                            send.button = 3;
                            network->send(&send);
                            break;
                        }
                    }
                    network->send(&send);
                    break;
                }

                case SDL_MOUSEBUTTONUP: {
                    send.type = TYPE_MOUSE_UP;
                    switch (userEvent.button.button) {
                        case SDL_BUTTON_LEFT: {
                            //printf("left click released\n");
                            send.button = 1;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_RIGHT: {
                            //printf("right click released\n");
                            send.button = 2;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_MIDDLE: {
                            //printf("middle click released\n");
                            send.button = 3;
                            network->send(&send);
                            break;
                        }
                    }
                    network->send(&send);
                    break;
                }
            }
        }
    }
}
