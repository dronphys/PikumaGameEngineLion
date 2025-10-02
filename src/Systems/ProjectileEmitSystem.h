//
// Created by kuyba on 9/29/2025.
//

#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H
#include "../Components/AllComponents.h"
#include <SDL.h>
#include "../Components/LifeSpanComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
class ProjectileEmitSystem: public System {
public:
    ProjectileEmitSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<ProjectileEmitterComponent>();
    }
    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event) {
        for (auto entity: GetSystemEntities()) {
            if (!entity.HasTag("player")){continue;}

            if (event.symbol == SDLK_SPACE) {
                ShootProjectile(entity);
            }
        }
    }
    // shot projectile from entity
    void ShootProjectile(Entity entity) {
        const auto& projectileEmitterC  = entity.GetComponent<ProjectileEmitterComponent>();
        const auto& transformC = entity.GetComponent<TransformComponent>();
        const auto& rigidBodyC = entity.GetComponent<RigidBodyComponent>();

        // choosing the direction to shoot projectile
        // default value is right
        int projDirX = +1;
        int projDirY =  0;
        if (rigidBodyC.velocity.x > 0) {
            projDirX = 1;
            projDirY = 0;
        }
        if (rigidBodyC.velocity.x < 0 ) {
            projDirX = -1;
            projDirY = 0;
        }
        if (rigidBodyC.velocity.y > 0 ) {
            projDirX = 0;
            projDirY = 1;
        }
        if (rigidBodyC.velocity.y < 0 ) {
            projDirX = 0;
            projDirY = -1;
        }

        glm::vec2 projectileVelocity = glm::vec2(projDirX * projectileEmitterC.projectileSpeed, projDirY * projectileEmitterC.projectileSpeed );
        glm::vec2 projectilePosition = transformC.position;
        if (entity.HasComponent<SpriteComponent>()) {
            const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
            projectilePosition.x += spriteComponent.width * transformC.scale.x * 0.5f;
            projectilePosition.y += spriteComponent.height * transformC.scale.y * 0.5f;
        }
        auto projectile = entity.registry->CreateEntity();
        projectile.Group("projectiles");
        projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0,1.0),0);
        projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
        projectile.AddComponent<SpriteComponent>("bullet-image",4,4,4);
        projectile.AddComponent<BoxColliderComponent>(4,4);
        projectile.AddComponent<ProjectileComponent>(projectileEmitterC.isFriendly, projectileEmitterC.hitPercentDamage);
        projectile.AddComponent<LifeSpanComponent>(projectileEmitterC.projectileDuration);
    }

    void Update(Registry& registry) {
        for (auto entity: GetSystemEntities()) {
            auto& projectileEmitterC  = entity.GetComponent<ProjectileEmitterComponent>();
            if (SDL_GetTicks() - projectileEmitterC.lastEmissionTime > projectileEmitterC.repeatFrequency
                &&projectileEmitterC.repeatFrequency > 0) {
                ShootProjectile(entity);
                projectileEmitterC.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};
#endif //PROJECTILEEMITSYSTEM_H
