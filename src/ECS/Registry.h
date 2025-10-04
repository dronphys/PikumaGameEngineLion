//
// Created by kuyba on 9/11/2025.
//

#ifndef REGISTRY_H
#define REGISTRY_H
#include <deque>
#include <memory>
#include "ECS.h"
#include <iostream>
#include <utility>

////////////////
////Registry
////////////////
class Registry {

private:
    int numEntities = 0;
    // Vector of component pools
    // each pool contains all the data for a certain component
    // Vector idx = component type id
    // Pool index = entity id
    std::vector<std::shared_ptr<IPool>> componentPools;

    // Vector of component signatures.
    // The signature lets us know which components are turned "on" for an entity
    // [vector index = entity id]
    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    //set of entities to be created and killed in update function
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;
    // entities, from which we remove some Component and now we need to remove them from certain systems
    std::vector<std::pair<Entity,std::shared_ptr<System>>> entitiesToBeRemovedFromSystems;
    std::vector<Entity> entitiesToBeAddedToSystems;

    // Entity tags
    std::unordered_map<std::string, Entity> entityPerTag;
    std::unordered_map<int, std::string> tagPerEntity;

    //Entity groups
    std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
    std::unordered_map<int, std::string> groupPerEntity;
    // Ids freed from entities and can be used for future
    std::deque<int> freeIds;

public:
    Registry() = default;
    Entity CreateEntity();
    void KillEntity(Entity entity);
    void Update();

    template <typename T, typename... TArgs>
    void AddComponent(const Entity entity, TArgs&&... args);

    template <typename TComponent>
    void RemoveComponent(const Entity entity);

    template <typename TComponent>
    bool HasComponent(const Entity entity) const;

    template <typename TComponent>
    TComponent& GetComponent(const Entity entity) const;

    // System  management
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs ...args);

    template <typename TSystem>
    void RemoveSystem();

    template <typename TSystem>
    bool HasSystem() const;

    template <typename TSystem>
    TSystem& GetSystem() const;

    //Checks the component signature of an entity and add the entity to the systems
    // that are interested in it
    void AddEntityToSystems(Entity entity);
    void RemoveEntityFromSystems(Entity entity);

    // Entity tag management
    void TagEntity(Entity entity, const std::string& tag);
    bool EntityHasTag(Entity entity, const std::string& tag) const;
    Entity GetEntityByTag(const std::string& tag) const;
    void RemoveEntityTag(Entity entity);

    // Entity group management
    void GroupEntity(Entity entity, const std::string& group);
    bool EntityInGroup(Entity entity, const std::string& group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
    void RemoveEntityGroup(Entity entity);

    std::string GetEntityTag(Entity entity) const;

};



template<typename TComponent, typename ... TArgs>
void Registry::AddComponent(const Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }
    if (!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }
    std::shared_ptr<Pool<TComponent>> componentPool
    = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    componentPool->Set(entityId, TComponent(std::forward<TArgs>(args)...));

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityComponentSignatures.size() + 1);
    }

    entityComponentSignatures[entityId].set(componentId);

    // if we had an existent entity and adding component to it
    // we need to add to the systems
    entitiesToBeAddedToSystems.push_back(entity);

    Logger::Log("Component id = "
        + std::to_string(componentId)
        + " has been added to entity id: "
        + std::to_string(entityId));
}

template<typename TComponent>
void Registry::RemoveComponent(const Entity entity) {
    if (!HasComponent<TComponent>(entity)) {return;} // do nothing if there is no such component

    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    entityComponentSignatures[entityId].set(componentId, false);

    // remove entity from component pool
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    componentPool->Remove(entityId);

    // We have to remove entity from Systems if the components no longer satisfy this system
    const auto& entityComponentSignature = entityComponentSignatures[entityId];
    for (auto& [id,system]: systems) {
        const auto& systemComponentSignature = system->GetComponentSignature();

        // we check if an entity is not in the system we remove this system
        // if entity wasn't in the system, nothing bad will happen.
        const bool notInSystem = (entityComponentSignature & systemComponentSignature) != systemComponentSignature;
        if (notInSystem) {
            system->RemoveEntityFromSystem(entity);
        }
    }

    Logger::Log("Component id = " + std::to_string(componentId) + " has been removed from entity id: " + std::to_string(entityId));
}

template<typename TComponent>
bool Registry::HasComponent(const Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    return entityComponentSignatures[entityId].test(componentId);

}
//TODO check if I can remove static_pointer_cast
template<typename TComponent>
TComponent& Registry::GetComponent(const Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entity.GetId());
    //return componentPools[componentId]->Get(entity.GetId());
}

template<typename TSystem, typename ... TArgs>
void Registry::AddSystem(TArgs... args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem() {
    systems.erase(std::type_index(typeid(TSystem)));
}

template<typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem)) != systems.end());
}

// This function might be wrong
template<typename TSystem>
TSystem& Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}



#endif //REGISTRY_H
