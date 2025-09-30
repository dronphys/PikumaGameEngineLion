//
// Created by kuyba on 9/29/2025.
//

#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H
#include <glm/glm.hpp>
#include <SDL.h>
struct ProjectileEmitterComponent {
    int projectileSpeed;
    int repeatFrequency; // in ms
    int projectileDuration; // in ms for how many seconds the projectile is alive
    int hitPercentDamage;
    bool isFriendly;
    int lastEmissionTime;

    ProjectileEmitterComponent
    ( int projectileSpeed = 150
    , int repeatFrequency = 3000
    , int projectileDuration = 5000
    , int hitPercentDamage = 10
    , bool isFriendly = false
    )
    : projectileSpeed(projectileSpeed)
    , repeatFrequency(repeatFrequency)
    , projectileDuration(projectileDuration)
    , hitPercentDamage(hitPercentDamage)
    , isFriendly(isFriendly)
    , lastEmissionTime(SDL_GetTicks())
    {}

};
#endif //PROJECTILEEMITTERCOMPONENT_H
