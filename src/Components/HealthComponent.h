//
// Created by kuyba on 9/30/2025.
//

#ifndef PIKUMAGAMEENGINELION_HEALTHCOMPONENT_H
#define PIKUMAGAMEENGINELION_HEALTHCOMPONENT_H
struct HealthComponent {
    int healthPercentage;

    HealthComponent(int healthPercentage = 0)
        :healthPercentage(healthPercentage){}
};
#endif //PIKUMAGAMEENGINELION_HEALTHCOMPONENT_H