//
// Created by kuyba on 9/30/2025.
//

#ifndef PIKUMAGAMEENGINELION_LIFESPANCOMPONENT_H
#define PIKUMAGAMEENGINELION_LIFESPANCOMPONENT_H
#include <SDL.h>
struct LifeSpanComponent {
    int duration;
    int startTime;
    LifeSpanComponent(int duration = 1000)
    :duration(duration), startTime(SDL_GetTicks())
    {}
};
#endif //PIKUMAGAMEENGINELION_LIFESPANCOMPONENT_H