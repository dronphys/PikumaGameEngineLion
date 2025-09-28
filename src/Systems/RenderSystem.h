//
// Created by kuyba on 9/13/2025.
//

#ifndef RENCERSYSTEM_H
#define RENCERSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/AllComponents.h"
#include "SDL.h"
#include "../AssetStore/AssetStore.h"
#include <algorithm>
class RenderSystem: public System {
public:
    RenderSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }
    void AddEntityToSystem(Entity entity) override {
        entities.push_back(entity);
        std::sort(entities.begin(), entities.end(), [](Entity a, Entity b) {
            return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex;
        });
    }
    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect camera) {

        for (auto entity: GetSystemEntities()) {
            const auto& transform = entity.GetComponent<TransformComponent>();
            const auto& sprite = entity.GetComponent<SpriteComponent>();

            // Set the source rectangle.
            SDL_Rect srcRect = sprite.srcRect;

            SDL_Rect dstRect = {
            static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
            static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)) ,
            static_cast<int>(sprite.width * transform.scale.x),
            static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_RenderCopyEx(
                renderer,
                assetStore->GetTexture(sprite.assetId),
                &srcRect,
                &dstRect,
                transform.rotation,
                nullptr,
                SDL_FLIP_NONE
                );
        }
    }

};

#endif //RENCERSYSTEM_H
