//
// Created by kuyba on 9/21/2025.
//

#ifndef RENDERCOLLISIONSYSTEM_H
#define RENDERCOLLISIONSYSTEM_H
#include <SDL.h>
#include "../Components/AllComponents.h"
#include "../ECS/ECS.h"

class RenderCollisionSystem: public System {
public:
    RenderCollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
        for (auto entity: GetSystemEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& boxCollider = entity.GetComponent<BoxColliderComponent>();

            int green = 255;

            SDL_SetRenderDrawColor(renderer, 255, green, 0, 100);
            SDL_Rect rect {
                static_cast<int>(transform.position.x + boxCollider.offset.x - camera.x),
                static_cast<int>(transform.position.y + boxCollider.offset.y - camera.y),
                static_cast<int>(boxCollider.width),
                static_cast<int>(boxCollider.height)};


            SDL_RenderDrawRect(renderer, &rect);

        }
    }
};

#endif //RENDERCOLLISIONSYSTEM
