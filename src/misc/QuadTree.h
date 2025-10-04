//
// Created by kuyba on 10/4/2025.
//

#ifndef QUADTREE_H
#define QUADTREE_H
#include <glm/glm.hpp>
#include <vector>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include <memory>
class Rect {
public:
    float x;
    float y;
    float w;
    float h;
    Rect(float x, float y, float width, float height):
    x(x), y(y), w(width), h(height)
    {}

    bool contains(float px, float py) const {
        return (px >= x && px <= x + w &&
                py >= y && py <= y + h);
    }

    bool intersects(const Rect& other) const {
        return !(other.x > x + w ||
                 other.x + other.w < x ||
                 other.y > y + h ||
                 other.y + other.h < y);
    }
};

class QuadTreeNode {
private:
    const int MAX_CAPACITY = 4;
    Rect rect;

    std::unique_ptr<QuadTreeNode> topRight;
    std::unique_ptr<QuadTreeNode> bottomRight;
    std::unique_ptr<QuadTreeNode> bottomLeft;
    std::unique_ptr<QuadTreeNode> topLeft;
    bool isDivided;
    std::vector<Entity> data;
public:
    QuadTreeNode(Rect rect):
    rect(rect),
    isDivided(false)
    {data.reserve(MAX_CAPACITY);}

public:
    bool Insert(Entity entity) {
        auto& pos = entity.GetComponent<TransformComponent>().position;
        auto& collider = entity.GetComponent<BoxColliderComponent>();

        Rect entityRect(pos.x, pos.y, collider.width, collider.height);
        if (!rect.intersects(entityRect)) {
            return false; // entity is completely outside this node
        }
        if (data.size() < MAX_CAPACITY) {
            data.push_back(entity);
            return true;
        }
        if (!isDivided) {
            Subdivide();
        }
        return InsertIntoChildren(entity);
    }
    bool InsertIntoChildren(Entity entity) {
        return (topRight->Insert(entity) ||
                bottomRight->Insert(entity) ||
                bottomLeft->Insert(entity) ||
                topLeft->Insert(entity));
    }
    void Subdivide() {
        float hw = rect.w / 2;
        float hh = rect.h / 2;
        topRight    = std::make_unique<QuadTreeNode>(Rect(rect.x + hw, rect.y, hw, hh));
        bottomRight = std::make_unique<QuadTreeNode>(Rect(rect.x + hw, rect.y + hh, hw, hh));
        bottomLeft  = std::make_unique<QuadTreeNode>(Rect(rect.x, rect.y + hh, hw, hh));
        topLeft     = std::make_unique<QuadTreeNode>(Rect(rect.x, rect.y, hw, hh));
        isDivided = true;
    }

    void Query(const Rect& boundary, std::vector<Entity>& found) {
        if (!rect.intersects(boundary)) {
            return;
        }
        for (auto entity: data) {
            auto pos = entity.GetComponent<TransformComponent>().position;
            auto& collider = entity.GetComponent<BoxColliderComponent>();
            Rect entityRect(pos.x, pos.y, collider.width, collider.height);
            if (boundary.intersects(entityRect)) {
                found.push_back(entity);
            }
        }
        if (isDivided) {
            topRight->Query(boundary, found);
            bottomRight->Query(boundary, found);
            bottomLeft->Query(boundary, found);
            topLeft->Query(boundary, found);
        }
    }
    void Clear() {
        data.clear();
        isDivided = false;
        topRight.reset();
        bottomRight.reset();
        bottomLeft.reset();
        topLeft.reset();
    }
};
#endif //QUADTREE_H
