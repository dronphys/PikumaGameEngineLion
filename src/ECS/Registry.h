//
// Created by kuyba on 9/11/2025.
//

#ifndef REGISTRY_H
#define REGISTRY_H
#include <memory>
#include "ECS.h"


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

    //set of entities to be created
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

public:
    Registry() = default;
    Entity CreateEntity();
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
};


template<typename TComponent, typename ... TArgs>
void Registry::AddComponent(const Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    if (componentId >= componentPools.size()) {
        componentPools.resize(componentPools.size() + 1, nullptr);
    }
    if (!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }
    std::shared_ptr<Pool<TComponent>> componentPool
    = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityComponentSignatures.size() + 1);
    }

    entityComponentSignatures[entityId].set(componentId);
    Logger::Log("Component id = "
        + std::to_string(componentId)
        + " has been added to entity id: "
        + std::to_string(entityId));
}

template<typename TComponent>
void Registry::RemoveComponent(const Entity entity) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    entityComponentSignatures[entityId].set(componentId, false);
    Logger::Log("Component id = " + std::to_string(componentId) + " has been removed from entity id: " + std::to_string(entityId));
}

template<typename TComponent>
bool Registry::HasComponent(const Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    return entityComponentSignatures[componentId].test(entityId);

}
//TODO check if I can remove static_pointer_cast
template<typename TComponent>
TComponent& Registry::GetComponent(const Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entity);
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
    // TODO check it later
    return systems.at(std::type_index(typeid(TSystem)));
}



#endif //REGISTRY_H
