
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
#include "../misc/QuadTree.h"
#include <vector>
#include "../misc/ThreadPool.h"
class CollisionSystem: public System {
private:
    QuadTreeNode tree;
    thread_pool pool;
    std::size_t chunk_size = 500;
    std::vector<std::future<void>> futures;
public:
    CollisionSystem():
    tree(Rect(-300,-300,5000,5000))
    {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    using VecEntIt =  std::vector<Entity>::const_iterator;

    void CalculateCollisions(VecEntIt firstIt, VecEntIt lastIt, EventBus& eventBus) {

        for (firstIt; firstIt != lastIt; ++firstIt) {
            std::vector<Entity> candidates;
            candidates.reserve(100);
            auto entity = *firstIt;
            const auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
            const auto& transform = entity.GetComponent<TransformComponent>();
            tree.Query(Rect(transform.position.x, transform.position.y, boxCollider.width,boxCollider.height),candidates);

            for (auto& otherEntity: candidates) {
                if (entity==otherEntity) {continue;}

                // so our entities colide only oce with each other.
                if (entity.GetId() > otherEntity.GetId()) {continue;}

                if (checkAABBCollision(entity, otherEntity)) {
                    // resolve collision emitting event
                    eventBus.EmitEvent<CollisionEvent>(entity,otherEntity);
                }
            }
            candidates.clear();
        }
    }


    void Update(std::unique_ptr<EventBus>& eventBus) {
        // check all collision of entities with all other entities
        const auto& entities = GetRefSystemEntities();
        auto entity1It = entities.begin();

        // by default entities do no collide.
        // we set it to true in collision detection
        for (auto entity: entities) {
            entity.GetComponent<BoxColliderComponent>().isColliding = false;
        }

        // reset tree
        tree.Clear();
        // Build a tree again
        for (auto entity: entities) {
            if (!tree.Insert(entity)) {
                std::cout <<"Error inserting in tree" << std::endl;
            }
        }
        EventBus* ev = eventBus.get();
        for (auto it = entities.begin(); it < entities.end(); ) {
            auto next = (std::distance(it, entities.end()) > chunk_size)
                      ? it + chunk_size
                      : entities.end();

            futures.push_back(pool.submit([this,it,next,ev]() {
                this->CalculateCollisions(it, next, *ev);
            }));

            it = next;
        }

        // wait for all
        for (auto& f : futures) f.get();
        futures.clear();

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

};

#endif //COLLISIONSYSTEM_H
