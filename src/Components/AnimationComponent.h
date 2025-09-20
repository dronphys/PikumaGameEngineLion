//
// Created by kuyba on 9/21/2025.
//

#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H
#include <SDL.h>
struct AnimationComponent {
    int numFrames;
    int currentFrame;
    int frameSpeedRate;
    bool isLoop;
    int startTime;

    AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool isLoop = true)
    : numFrames(numFrames),
    frameSpeedRate(frameSpeedRate),
    currentFrame(1),
    isLoop(isLoop),
    startTime(SDL_GetTicks())
    {}
};

#endif //ANIMATIONCOMPONENT_H
