//
// Created by kuyba on 10/4/2025.
//

#ifndef LINKCOMPONENT_H
#define LINKCOMPONENT_H
#include "glm/glm.hpp"
// forward declaring
#include "..//ECS/ECS.h"
struct LinkComponent {
    glm::vec2 offset;
    Entity host;
    LinkComponent(glm::vec2 offset = glm::vec2(0,0), Entity host = Entity(-1)) :
    offset(offset), host(host) {}
};
#endif //LINKCOMPONENT_H
