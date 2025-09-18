//
// Created by kuyba on 9/13/2025.
//

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include <string>
#include <SDL.h>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    SDL_Rect srcRect;
    //we need to always have default values
    SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int srcRectX = 0, int srcRectY = 0)
    : assetId(assetId) ,width(width), height(height), srcRect{srcRectX,srcRectY,width,height} {

    }
};
#endif //SPRITECOMPONENT_H
