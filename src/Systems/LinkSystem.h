//
// Created by kuyba on 10/4/2025.
//

#ifndef LINKSYSTEM_H
#define LINKSYSTEM_H
#include <algorithm>

#include "../Components/LinkComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class LinkSystem: public System {
public:
    LinkSystem() {
        RequireComponent<LinkComponent>();
        RequireComponent<TransformComponent>();
    }
    void Update() {
        for (auto& entity : entities) {
            auto& transform = entity.GetComponent<TransformComponent>();
            const auto& link = entity.GetComponent<LinkComponent>();
            if (link.host.HasComponent<TransformComponent>()) {
                const auto& hostTransform = link.host.GetComponent<TransformComponent>();
                transform.position = hostTransform.position + link.offset;
            }
        }
    }
};
#endif //LINKSYSTEM_H
