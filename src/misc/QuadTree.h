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
        if (data.size() < MAX_CAPACITY) {
            data.push_back(entity);
            return true;
        }
        isDivided = true;
        topRight = std::make_unique<QuadTreeNode>(Rect(rect.x + rect.w/2,rect.y, rect.w/2, rect.h/2));
        bottomRight = std::make_unique<QuadTreeNode>(Rect(rect.x + rect.w/2,rect.y + rect.h/2, rect.w/2, rect.h/2));
        bottomLeft = std::make_unique<QuadTreeNode>(Rect(rect.x,rect.y + rect.h/2, rect.w/2, rect.h/2));
        topLeft = std::make_unique<QuadTreeNode>(Rect(rect.x,rect.y, rect.w/2, rect.h/2));
        return (topRight->Insert(entity) ||
            bottomRight->Insert(entity) ||
                bottomLeft->Insert(entity)||
                    topLeft->Insert(entity));
    }

    void Query(const Rect& boundary, std::vector<Entity>& found) {
        if (!rect.intersects(boundary)) {
            return;
        }
        for (auto entity: data) {
            auto point = entity.GetComponent<TransformComponent>().position;
            if (rect.contains(point.x, point.y)) {
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
