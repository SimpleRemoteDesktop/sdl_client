//
// Created by user on 10/3/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SDL_CLIENT_QUEUE_H
#define SIMPLEREMOTEDESKTOP_SDL_CLIENT_QUEUE_H

#include <condition_variable>
#include <deque>
#include <SDL2/SDL_mutex.h>

template<typename T>
class Queue {
private:
    SDL_mutex *d_mutex;
    SDL_cond *d_condition;
    std::deque<T> d_queue;
public:
    Queue() {
        d_mutex = SDL_CreateMutex();
        d_condition = SDL_CreateCond();
    }
    void push(T const &value) {
        {
            SDL_LockMutex(this->d_mutex);
            d_queue.push_front(value);
        }
            SDL_CondSignal(this->d_condition);
        SDL_UnlockMutex(this->d_mutex);
    }

    T pop() {
        SDL_LockMutex(this->d_mutex);
        while(this->d_queue.empty()) {
            SDL_CondWait(this->d_condition, this->d_mutex);
        }
        T rc(std::move(this->d_queue.back()));
        this->d_queue.pop_back(); //TODO destroy frame ?
        SDL_UnlockMutex(this->d_mutex);
        return rc;
    }

};


#endif //SIMPLEREMOTEDESKTOP_SDL_CLIENT_QUEUE_H
