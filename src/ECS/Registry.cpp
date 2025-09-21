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
}

void Registry::Update() {
    for (const auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (const auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);

        entityComponentSignatures[entity.GetId()].reset();

        freeIds.push_back(entity.GetId());
    }
    entitiesToBeKilled.clear();

    for (auto& [entity, system]: entitiesToBeRemovedFromSystems) {
        system->RemoveEntityFromSystem(entity);
    }
    entitiesToBeRemovedFromSystems.clear();
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