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
class Registry{};



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


