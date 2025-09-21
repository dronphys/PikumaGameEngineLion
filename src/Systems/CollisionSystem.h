//
// Created by kuyba on 9/21/2025.
//

#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "../Components/AllComponents.h"
#include "../ECS/ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"
class CollisionSystem: public System {
public:
    CollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update() {
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
                    ResolveCollision(e1,e2);
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
    //     auto aPosition = a.GetComponent<TransformComponent>().position;
    //     auto bPosition = b.GetComponent<TransformComponent>().position;
    //     auto aOffset = a.GetComponent<BoxColliderComponent>().offset;
    //     auto bOffset = b.GetComponent<BoxColliderComponent>().offset;
    //     auto aWidth = a.GetComponent<BoxColliderComponent>().width;
    //     auto bWidth = b.GetComponent<BoxColliderComponent>().width;
    //     auto aHeight = a.GetComponent<BoxColliderComponent>().height;
    //     auto bHeight = b.GetComponent<BoxColliderComponent>().height;
    //     int aMinX = aPosition.x + aOffset.x;
    //     int aMinY = aPosition.y + aOffset.y;
    //
    //     int bMinX = bPosition.x + bOffset.x;
    //     int bMinY = bPosition.y + bOffset.y;
    //     int aMaxX = aMinX + aWidth;
    //     int aMaxY = aMinY + aHeight;
    //     int bMaxX = bMinX + bWidth;
    //     int bMaxY = bMinY + bHeight;
    //
    //
    //     int overlapX1 = aMaxX - bMinX;
    //     int overlapX2 = aMaxX - aMinX;
    //     int overlapY1 = aMaxY - bMinY;
    //     int overlapY2 = aMaxY - aMinY;
    //
    //     auto penetrationX = std::min(overlapX1, overlapX2);
    //     auto penetrationY = std::min(overlapY1, overlapY2);
    //
    //     if (penetrationX < penetrationY) {
    //         if (overlapX1 < overlapX2) {
    //             a.GetComponent<TransformComponent>().position.x -= penetrationX;
    //             b.GetComponent<TransformComponent>().position.x += penetrationX;
    //         }
    //         else {
    //             a.GetComponent<TransformComponent>().position.x += penetrationX;
    //             b.GetComponent<TransformComponent>().position.x -= penetrationX;
    //         }
    //     }
    //     else {
    //         if (overlapY1 < overlapY2 ) {
    //             a.GetComponent<TransformComponent>().position.y += penetrationY;
    //             b.GetComponent<TransformComponent>().position.y -= penetrationY;
    //         }
    //         else {
    //             a.GetComponent<TransformComponent>().position.y -= penetrationY;
    //             b.GetComponent<TransformComponent>().position.y += penetrationY;
    //         }
    //     }
    // }
};

#endif //COLLISIONSYSTEM_H
