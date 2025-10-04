//
// Created by kuyba on 9/6/2025.
//

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <glm/glm.hpp>
struct TransformComponent {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
    bool isFixed;
    TransformComponent(
        glm::vec2 position = glm::vec2(0,0),
        glm::vec2 scale = glm::vec2(1,1),
        double rotation = 0.0,
        bool isFixed = false) :
        position(position),scale(scale), rotation(rotation), isFixed(isFixed) {}
};





#endif //TRANSFORMCOMPONENT_H
