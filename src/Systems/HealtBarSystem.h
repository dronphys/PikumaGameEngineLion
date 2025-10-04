//
// Created by kuyba on 10/4/2025.
//

#ifndef HEALTBARSYSTEM_H
#define HEALTBARSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/AllComponents.h"
#include "../ECS/Registry.h"
class HealthBarSystem: public System {
public:
    HealthBarSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<LinkComponent>();
    }

    void Update(Registry& registry) {
        auto entities = registry.GetEntitiesByGroup("hpBar");
        for (auto& entity: entities) {

            auto& linkComponent = entity.GetComponent<LinkComponent>();
            auto& host = linkComponent.host;
            if (!host.HasComponent<HealthComponent>()) {
                entity.Kill();
                continue;
            }
            const auto hostHpPercentage = host.GetComponent<HealthComponent>().healthPercentage;
            auto& textComponent = entity.GetComponent<TextLabelComponent>();
            textComponent.text = std::to_string(hostHpPercentage) + "%";
        }
    }
};
#endif //HEALTBARSYSTEM_H
