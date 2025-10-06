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

void Entity::Tag(const std::string &tag) {
    registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string &tag) const {
    return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string &group) {
    registry->GroupEntity(*this, group);
}

bool Entity::InGroup(const std::string &group) const {
    return registry->EntityInGroup(*this, group);
}

void System::AddEntityToSystem(Entity entity) {
    // we add an entity only if it's not in the system already
    if (std::find(entities.begin(), entities.end(), entity) == entities.end()) {
        entities.push_back(entity);
    }
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





