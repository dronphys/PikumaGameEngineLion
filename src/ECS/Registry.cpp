//
// Created by kuyba on 9/11/2025.
//

#include "Registry.h"
Entity Registry::CreateEntity() {
    int entityId;

    if (freeIds.empty()) {
        entityId = numEntities++;
        if (entityId >= entitiesToBeAdded.size() ) {
            entityComponentSignatures.resize(entityId*2);
        }
    }
    else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }


    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string( entityId));
    return entity;
}

void Registry::KillEntity(Entity entity) {
    entitiesToBeKilled.insert(entity);
    Logger::Log("Entity " + std::to_string(entity.GetId()) + " was killed");
}

void Registry::Update() {
    for (const auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (const auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();
        // remove entity from component pools

        for (auto pool:componentPools) {
            // to remove only of pool exists
            if (pool) {
                pool->RemoveEntityFromPool(entity.GetId());
            }

        }

        freeIds.push_back(entity.GetId());

        RemoveEntityTag(entity);
        RemoveEntityGroup(entity);
    }
    entitiesToBeKilled.clear();

    for (auto& [entity, system]: entitiesToBeRemovedFromSystems) {
        system->RemoveEntityFromSystem(entity);
    }
    entitiesToBeRemovedFromSystems.clear();

    // remove any traces of entity in groups and tags

}

void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    for (auto& [id,system]: systems) {
        const auto& systemComponentSignature = system->GetComponentSignature();

        const bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
        if (isInterested) {
            system->AddEntityToSystem(entity);
        }
    }
}
void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto& [id,system]: systems) {
        system->RemoveEntityFromSystem(entity);
    }
}

void Registry::TagEntity(Entity entity, const std::string &tag) {
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string &tag) const {
    if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end()) {
        return false;
    }
    if (entityPerTag.find(tag) == entityPerTag.end()) {
        return false;
    }
    return entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string &tag) const {
    return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity) {
    auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end()) {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

void Registry::GroupEntity(Entity entity, const std::string &group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityInGroup(Entity entity, const std::string &group) const {
    auto groupEntities = entitiesPerGroup.find(group);
    if (groupEntities != entitiesPerGroup.end()) {
        return groupEntities->second.find(entity.GetId()) != groupEntities->second.end();
    }
    return false;
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string &group) const {
    auto setOfEntites = entitiesPerGroup.at(group);
    std::vector<Entity> entities;
    entities.reserve(setOfEntites.size());
    entities.insert(entities.end(), setOfEntites.begin(), setOfEntites.end());
    return entities;

}

// here might be a problem. Not how gustavo implemented
void Registry::RemoveEntityGroup(Entity entity) {
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity != groupPerEntity.end()) {
        auto groupName = groupedEntity->second;
        entitiesPerGroup.find(groupName)->second.erase(entity);
        groupPerEntity.erase(groupedEntity);
    }
}

std::string Registry::GetEntityTag(Entity entity) const {
    if (tagPerEntity.find(entity.GetId()) != tagPerEntity.end()) {
        return tagPerEntity.find(entity.GetId())->second;
    }
    return "no tag";
}
