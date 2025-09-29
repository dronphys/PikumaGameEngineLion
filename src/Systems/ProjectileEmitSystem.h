//
// Created by kuyba on 9/29/2025.
//

#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H
#include "../Components/AllComponents.h"
#include <SDL.h>
class ProjectileEmitSystem: public System {
public:
    ProjectileEmitSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<ProjectileEmitterComponent>();
    }
    void Update(Registry& registry) {
        for (auto entity: GetSystemEntities()) {
            auto& projectileComponent  = entity.GetComponent<ProjectileEmitterComponent>();
            const auto& transformComponent = entity.GetComponent<TransformComponent>();

            if (SDL_GetTicks() - projectileComponent.lastEmissionTime > projectileComponent.repeatFrequency) {
                auto projectile = registry.CreateEntity();
                glm::vec2 projectilePosition = transformComponent.position;
                if (entity.HasComponent<SpriteComponent>()) {
                    const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += spriteComponent.width * transformComponent.scale.x * 0.5f;
                    projectilePosition.y += spriteComponent.height * transformComponent.scale.y * 0.5f;
                }


                projectile.AddComponent<TransformComponent>(
                    projectilePosition, glm::vec2(1.0,1.0),
                    0);
                projectile.AddComponent<RigidBodyComponent>(projectileComponent.projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-image",4,4,4);
                projectile.AddComponent<BoxColliderComponent>(4,4);

                projectileComponent.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};
#endif //PROJECTILEEMITSYSTEM_H
