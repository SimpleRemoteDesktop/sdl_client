#include "input.h"
#include "network.h"
#include "video_surface.h"

#define MOUSE_POLLING_INTERVAL 5

extern Network *network;

InputHandler::InputHandler(Network *network, PlayerManager *appManager, bool withRelativeMouse) {
    this->network = network;
    this->appManager = appManager;
    this->withRelativeMouse = withRelativeMouse;


}

void InputHandler::run() {
    this->isRunning = true;
    m_MouseMoveTimer = SDL_AddTimer(MOUSE_POLLING_INTERVAL, InputHandler::mouseMoveTimerCallback, this);
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
                    this->appManager->getScreenSize(&w, &h);
                    if(this->withRelativeMouse) {
                        SDL_AtomicAdd(&mouseMotionType, TYPE_MOUSE_RELATIVE_MOTION);
                        SDL_AtomicAdd(&mouseDeltaX, (float) userEvent.motion.xrel / (float) w);
                        SDL_AtomicAdd (&mouseDeltaY, (float) userEvent.motion.yrel / (float) h);
                    } else {
                        SDL_AtomicAdd(&mouseMotionType, TYPE_MOUSE_MOTION);
                        SDL_AtomicAdd(&mouseDeltaX, (float) userEvent.motion.x / (float) w);
                        SDL_AtomicAdd(&mouseDeltaY, (float) userEvent.motion.y / (float) h);
                    }
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

Uint32 InputHandler::mouseMoveTimerCallback(Uint32 interval, void *param) {
    auto me = reinterpret_cast<InputHandler*>(param);
    struct Message send;
    send.type = SDL_AtomicSet(&me->mouseMotionType, 0);
    send.x = (float)SDL_AtomicSet(&me->mouseDeltaX, 0);
    send.y = (float)SDL_AtomicSet(&me->mouseDeltaY, 0);

    if (send.type != 0 && send.x != 0 || send.y != 0) {
        me->network->send(&send);
    }

    return interval;
}
