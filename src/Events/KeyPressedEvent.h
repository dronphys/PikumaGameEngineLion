//
// Created by kuyba on 9/26/2025.
//

#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H
#include "../EventBus/Event.h"
class KeyPressedEvent: public Event {
public:
    SDL_Keycode symbol;
    KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}

};
#endif //KEYPRESSEDEVENT_H
