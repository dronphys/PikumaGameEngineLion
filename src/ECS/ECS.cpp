//
// Created by kuyba on 9/6/2025.
//

#include "ECS.h"
#include <algorithm>

#include "Registry.h"

int IComponent::nextId = 0; // definng static variable in IComponent class

void Entity::Kill() {
    registry->KillEntity(*this);
}

int Entity::GetId() const {
    return id;
}

bool Entity::operator==(Entity other) const {
    return id == other.id;
}

bool Entity::operator<(Entity other) const {
    return this->id < other.id;
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(const Entity entity) {
    entities.erase(
        std::remove(entities.begin(), entities.end(), entity), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature & System::GetComponentSignature() const {
    return componentSignature;
}





