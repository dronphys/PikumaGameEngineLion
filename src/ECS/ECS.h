//
// Created by kuyba on 9/6/2025.
//

#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>
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

#endif //ECS_H


