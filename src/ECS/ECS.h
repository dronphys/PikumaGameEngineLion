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
using Signature =  std::bitset<MAX_COMPONENTS> ;

//Forward declaration of class Registry in Registry.h
class Registry;

class Entity {
private:
    int id;

public:
    Registry* registry;

    Entity(int id_) :id(id_) {
    };

    void Kill();
    int GetId() const;
    bool operator==(Entity other) const;
    bool operator<(Entity other) const;


    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs&& ...args);

    template <typename TComponent>
    void RemoveComponent();

    template <typename TComponent>
    bool HasComponent () const;

    template <typename TComponent>
    TComponent& GetComponent() const;

    void Tag(const std::string& tag);
    bool HasTag(const std::string& tag) const;
    void Group(const std::string& group);
    bool InGroup(const std::string& group) const;
    std::string GetTag() const;
};

struct IComponent {
protected:
    static int nextId;
};

// Used to assign a unique id to a component type
template <typename T>
class Component: public IComponent {
public:
    static int GetId() {
        static auto id = nextId++;
        return id;
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
        data[index] = std::move(object);
    }

    T& Get(int index) {
        // why here static cast? later try to remove
        return static_cast<T&> (data[index]);
    }
    T& operator[](int index) const {
        return data[index];
    }
};


// system processes entities that contain a specific signature
class System {
protected:
    Signature componentSignature;
    std::vector<Entity> entities;
public:
    System() = default;
    virtual ~System() = default;

    virtual void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;

    template <typename TComponent>
    void RequireComponent();
};

template<typename TComponent, typename ... TArgs>
void Entity::AddComponent(TArgs &&...args) {
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::RemoveComponent() {
    registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent() const {
    return registry->GetComponent<TComponent>(*this);
}

template<typename TComponent>
void System::RequireComponent(){
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}



#endif //ECS_H


