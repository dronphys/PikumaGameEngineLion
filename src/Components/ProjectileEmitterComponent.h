//
// Created by kuyba on 9/29/2025.
//

#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H
#include <glm/glm.hpp>
#include <SDL.h>
struct ProjectileEmitterComponent {
    glm::vec2 projectileVelocity;
    int repeatFrequency; // in ms
    int projectileDuration; // in ms for how many seconds the projectile is alive
    int hitPercentDamage;
    bool isFriendly;
    int lastEmissionTime;

    ProjectileEmitterComponent
    ( glm::vec2 projectileVelocity = glm::vec2(0.0f)
    , int repeatFrequency = 3000
    , int projectileDuration = 5000
    , int hitPercentDamage = 10
    , bool isFriendly = false
    )
    : projectileVelocity(projectileVelocity)
    , repeatFrequency(repeatFrequency)
    , projectileDuration(projectileDuration)
    , hitPercentDamage(hitPercentDamage)
    , isFriendly(isFriendly)
    , lastEmissionTime(SDL_GetTicks())
    {}

};
#endif //PROJECTILEEMITTERCOMPONENT_H
