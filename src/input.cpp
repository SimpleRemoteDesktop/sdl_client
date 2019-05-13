#include "input.h"
#include "network.h"
#include "video_surface.h"

#define MOUSE_POLLING_INTERVAL 5

extern Network *network;
SDL_atomic_t mouseRelativeMotionX;
SDL_atomic_t mouseRelativeMotionY;
SDL_atomic_t mouseAbsMotionX;
SDL_atomic_t mouseAbsMotionY;



InputHandler::InputHandler(Network *network, PlayerManager *appManager, bool withRelativeMouse) {
    this->network = network;
    this->appManager = appManager;
    this->withRelativeMouse = withRelativeMouse;
    SDL_AtomicSet(&mouseRelativeMotionX, 0);
    SDL_AtomicSet(&mouseRelativeMotionY, 0);
    SDL_AtomicSet(&mouseAbsMotionX, 0);
    SDL_AtomicSet(&mouseAbsMotionY, 0);

}

void InputHandler::run() {
    this->isRunning = true;
    SDL_AddTimer(MOUSE_POLLING_INTERVAL, InputHandler::mouseMoveTimerCallback, this);
    while (this->isRunning) {
        SDL_Event userEvent;
        while (SDL_WaitEvent(&userEvent)) {
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
                                       if(this->withRelativeMouse) {
                        SDL_AtomicSet(&mouseRelativeMotionX, (int) SDL_AtomicGet(&mouseRelativeMotionX) + userEvent.motion.xrel);
                        SDL_AtomicSet(&mouseRelativeMotionY, (int)  SDL_AtomicGet(&mouseRelativeMotionY) + userEvent.motion.yrel);
                    } else {
                        SDL_AtomicSet(&mouseAbsMotionX, userEvent.motion.x);
                        SDL_AtomicSet(&mouseAbsMotionY, userEvent.motion.y);
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
                            send.button = 2;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_MIDDLE: {
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
                            send.button = 1;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_RIGHT: {
                            send.button = 2;
                            network->send(&send);
                            break;
                        }
                        case SDL_BUTTON_MIDDLE: {
                            send.button = 3;
                            network->send(&send);
                            break;
                        }
                    }
                    network->send(&send);
                    break;
                }

                case SDL_MOUSEWHEEL: {
                    send.type = TYPE_MOUSE_WHEEL;
                    send.x = userEvent.wheel.x;
                    send.y = userEvent.wheel.y;
                    network->send(&send);
                    break;
                }
            }
        }
    }
}

Uint32 InputHandler::mouseMoveTimerCallback(Uint32 interval, void *param) {
    auto me = reinterpret_cast<InputHandler*>(param);
    int w, h;
    me->appManager->getScreenSize(&w, &h);
    struct Message send;
    if(me->withRelativeMouse) {
	send.type = TYPE_MOUSE_RELATIVE_MOTION;
	send.x = (float) SDL_AtomicGet(&mouseRelativeMotionX);
	send.y = (float) SDL_AtomicGet(&mouseRelativeMotionY);
	SDL_AtomicSet(&mouseRelativeMotionX, 0);
	SDL_AtomicSet(&mouseRelativeMotionY, 0);
    } else {
	send.type = TYPE_MOUSE_MOTION;
	send.x = (float) SDL_AtomicGet(&mouseAbsMotionX) / (float) w;
	send.y = (float) SDL_AtomicGet(&mouseAbsMotionY) / (float) h;
    }

        me->network->send(&send);

    return interval;
}
