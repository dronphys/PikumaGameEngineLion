//
// Created by kuyba on 9/30/2025.
//

#ifndef PIKUMAGAMEENGINELION_LIFESPANSYSTEM_H
#define PIKUMAGAMEENGINELION_LIFESPANSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/LifeSpanComponent.h"
#include <SDL.h>
class LifeSpanSystem: public System {
public:
    LifeSpanSystem() {
        RequireComponent<LifeSpanComponent>();
    }
    void Update() {
        for (auto entity: GetSystemEntities()) {
            auto lifeSpan = entity.GetComponent<LifeSpanComponent>();
            if (SDL_GetTicks() - lifeSpan.startTime > lifeSpan.duration) {
                entity.Kill();
            }
        }
    }
};
#endif //PIKUMAGAMEENGINELION_LIFESPANSYSTEM_H