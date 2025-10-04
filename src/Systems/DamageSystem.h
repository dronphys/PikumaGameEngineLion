//
// Created by kuyba on 9/26/2025.
//

#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/AllComponents.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"
class DamageSystem: public System {
public:
    DamageSystem() {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event) {
        Entity a = event.a;
        Entity b = event.b;
        // Logger::Log("Damage system received a  collision between entities " +
        //     std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

        if (a.InGroup("projectiles")&& b.HasTag("player")) {
            OnProjectileHitsPlayer(a,b); // a is projectile b is the player
        }
        if (b.InGroup("projectiles")&& a.HasTag("player")) {
            OnProjectileHitsPlayer(b,a); // b is the projectile a is the player
        }

        if (a.InGroup("projectiles")&& b.InGroup("enemies")) {
            OnProjectileHitsEnemy(a,b);
        }
        if (b.InGroup("projectiles")&& a.InGroup("enemies")) {
            OnProjectileHitsEnemy(b,a);
        }
    }

    void OnProjectileHitsPlayer(Entity projectile, Entity player) {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
        projectile.GetComponent<BoxColliderComponent>().isColliding = true;
        player.GetComponent<BoxColliderComponent>().isColliding = true;
        if (!projectileComponent.isFriendly) {
            auto& health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentDamage;

            if (health.healthPercentage <= 0) {
                player.Kill();
            }
            projectile.Kill();
        }
        Logger::Log("Projectile Hits player");
    }

    void OnProjectileHitsEnemy(Entity projectile, Entity enemy) {
        projectile.GetComponent<BoxColliderComponent>().isColliding = true;
        enemy.GetComponent<BoxColliderComponent>().isColliding = true;
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
        if (!projectileComponent.isFriendly) {
            return; // only damage enemy if projectile component was friendly/
        }
        auto& health = enemy.GetComponent<HealthComponent>();
        health.healthPercentage -= projectileComponent.hitPercentDamage;

        if (health.healthPercentage <= 0) {
            enemy.Kill();
        }
        projectile.Kill();
        Logger::Log("Projectile Hits enemy");
    }

    void Update() {}
};


#endif //DAMAGESYSTEM_H
