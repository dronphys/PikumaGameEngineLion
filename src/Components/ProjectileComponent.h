//
// Created by kuyba on 9/30/2025.
//

#ifndef PIKUMAGAMEENGINELION_PROJECTILECOMPONENT_H
#define PIKUMAGAMEENGINELION_PROJECTILECOMPONENT_H
#include <SDL.h>
struct ProjectileComponent {
    bool isFriendly;
    int hitPercentDamage;

    ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0)
    :isFriendly(isFriendly)
    ,hitPercentDamage(hitPercentDamage)
    {}
};
#endif //PIKUMAGAMEENGINELION_PROJECTILECOMPONENT_H