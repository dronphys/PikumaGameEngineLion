//
// Created by kuyba on 9/21/2025.
//

#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "../Components/AllComponents.h"
#include "../ECS/ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem: public System {
public:
    CollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(std::unique_ptr<EventBus>& eventBus) {
        // check all collision of entities with all other entities
        auto entities = GetSystemEntities();
        auto entity1It = entities.begin();

        // by default entities do no collide.
        // we set it to true in collision detection
        for (auto entity: entities) {
            entity.GetComponent<BoxColliderComponent>().isColliding = false;
        }


        for (; entity1It != entities.end(); entity1It++) {
            auto entity2It = entity1It;
            Entity e1 = *entity1It;
            for (;entity2It != entities.end(); entity2It++ ) {
                Entity e2 = *entity2It;
                if (e1==e2) continue;
                if (checkAABBCollision(e1,e2)) {
                    //ResolveCollision(e1,e2);
                    eventBus->EmitEvent<CollisionEvent>(e1,e2);
                }

            }

        }
    }

    bool checkAABBCollision (Entity a, Entity b) {
        auto aPosition = a.GetComponent<TransformComponent>().position;
        auto bPosition = b.GetComponent<TransformComponent>().position;
        auto aOffset = a.GetComponent<BoxColliderComponent>().offset;
        auto bOffset = b.GetComponent<BoxColliderComponent>().offset;
        auto aWidth = a.GetComponent<BoxColliderComponent>().width;
        auto bWidth = b.GetComponent<BoxColliderComponent>().width;
        auto aHeight = a.GetComponent<BoxColliderComponent>().height;
        auto bHeight = b.GetComponent<BoxColliderComponent>().height;

        int aMinX = aPosition.x + aOffset.x;
        int aMinY = aPosition.y + aOffset.y;

        int bMinX = bPosition.x + bOffset.x;
        int bMinY = bPosition.y + bOffset.y;
        int aMaxX = aMinX + aWidth;
        int aMaxY = aMinY + aHeight;
        int bMaxX = bMinX + bWidth;
        int bMaxY = bMinY + bHeight;

        if ((aMaxX < bMinX) || (aMinX > bMaxX))
            return false;

        if  ((aMaxY < bMinY) || (aMinY > bMaxY))
            return false;

        return true;
    }

    void ResolveCollision(Entity a, Entity b) {
        a.GetComponent<BoxColliderComponent>().isColliding = true;
        b.GetComponent<BoxColliderComponent>().isColliding = true;
        b.RemoveComponent<BoxColliderComponent>();
        //Logger::Log("Entity " + std::to_string(a.GetId()) +  " collides with entity " + std::to_string(b.GetId()));
    }
};

#endif //COLLISIONSYSTEM_H
