//
// Created by kuyba on 9/11/2025.
//

#include "Registry.h"
Entity Registry::CreateEntity() {
    int entityId;
    entityId = numEntities++;
    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    if (entityId >= entitiesToBeAdded.size() ) {
        entityComponentSignatures.resize(entityId*2);
    }

    Logger::Log("Entity created with id = " + std::to_string( entityId));
    return entity;
}

void Registry::Update() {
    for (const auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
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
