//
// Created by kuyba on 9/6/2025.
//

#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <set>
#include "../Logger/Logger.h"

constexpr unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;



class Entity {
private:
    int id;
public:
    Entity(int id_)
        :id(id_){};
    int GetId() const;
    bool operator==(Entity other) const;
    bool operator<(Entity other) const;
};

struct IComponent {
protected:
    static int nextId;
};

// Used to assign a unique id to a component type
template <typename T>
class Component: public IComponent {
    static int GetId() {
        return nextId++;
    }
};

///////////////
///Pool
//////////////
/// Type erasior idiom
class IPool {
public:
    virtual ~IPool() = default;
};

template <typename T>
class Pool: public IPool {
private:
    std::vector<T> data;
public:
    Pool(int size = 100) {
        data.resize(size);
    }
    ~Pool() override = default;

    bool isEmpty() const {
        return data.empty();
    }

    int GetSize() const {
        return data.size();
    }

    void Resize(int n) {
        data.resize(n);
    }

    void Clear() {
        data.clear();
    }

    void Add(T object) {
        data.push_back(object);
    }

    void Set(int index, T object) {
        data[index] = object;
    }

    T& Get(int index) const {
        // why here static cast? later try to remove
        return static_cast<T&> (data[index]);
    }
    T& operator[](int index) const {
        return data[index];
    }
};


// system processes entities that contain a specific signature
class System {
private:
    Signature componentSignature;
    std::vector<Entity> entities;
public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;

    template <typename TComponent>
    void RequireComponent();
};
template<typename TComponent>
void System::RequireComponent(){
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

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
    std::vector<IPool*> componentPools;

    // Vector of component signatures.
    // The signature lets us know which components are turned "on" for an entity
    // [vector index = entity id]
    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, System*> systems;

    //set of entities to be created
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;
    public:
    Registry() = default;
    Entity CreateEntity();
    void Update();
    void AddEntityToSystem(Entity entity);

    template <typename T, typename... TArgs>
    void AddComponent(const Entity entity, TArgs&&... args);
};


template<typename TComponent, typename ... TArgs>
void Registry::AddComponent(const Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    if (componentId >= componentPools.size()) {
        componentPools.resize(componentPools.size() + 1, nullptr);
    }
    if (!componentPools[componentId]) {
        Pool<TComponent>* newComponentPool = new Pool<TComponent>();
        componentPools[componentId] = newComponentPool;
    }
    Pool<TComponent>* componentPool = componentPools[componentId];

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);
}
#endif //ECS_H


