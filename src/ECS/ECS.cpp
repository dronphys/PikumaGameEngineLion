//
// Created by kuyba on 9/6/2025.
//

#include "ECS.h"
#include <algorithm>
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

Entity Registry::CreateEntity() {
    int entityID;
    entityID = numEntities++;
    Entity entity(entityID);
    entitiesToBeAdded.insert(entity);


    Logger::Log("Entity created with id = " + std::to_string( entityID));
    return entity;
}

void Registry::Update() {
    // TODO: add the entities to be created
    // TODO: Remove entities that are to be killed
}

void Registry::AddEntityToSystem(Entity entity) {

}




