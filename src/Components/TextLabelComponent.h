//
// Created by kuyba on 10/4/2025.
//

#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H
#include "glm/glm.hpp"
#include <string>

#include <SDL.h>

struct TextLabelComponent {
    std::string text;
    std::string assetId;
    SDL_Color color;
    TextLabelComponent(
        std::string text = "",
        std::string assetId = "",
        SDL_Color color = {0,0,0}):
    text(text),
    assetId(assetId),
    color(color)
    {}
};
#endif //TEXTLABELCOMPONENT_H
