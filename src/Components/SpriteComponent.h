//
// Created by kuyba on 9/13/2025.
//

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
struct SpriteComponent {
    int width;
    int height;
    //we need to always have default values
    SpriteComponent(int width = 0, int height = 0)
    : width(width), height(height) {};
};
#endif //SPRITECOMPONENT_H
