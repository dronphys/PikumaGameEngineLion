//
// Created by kuyba on 9/28/2025.
//

#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "SDL.h"

class CameraMovementSystem: public System {
    public:
    CameraMovementSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }
    void Update(SDL_Rect& camera) {
        for (auto entity: entities) {
            auto transform = entity.GetComponent<TransformComponent>();
            camera.x = transform.position.x - camera.w/2;
            camera.y = transform.position.y - camera.h/2;

        }
    }
};
#endif //CAMERAMOVEMENTSYSTEM_H
