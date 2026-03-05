#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <type_traits>
#include <string>

class Component;  // Forward declaraction

class Entity {
private:
    int id = 1;
    std::string name = "entity";
    
protected:
    std::vector<std::unique_ptr<Component>> components;

public:
    explicit Entity(const std::string& entityName = "entity")
            : name(entityName) {}
    
    virtual ~Entity() = default;
    
    int GetId() const { return id; }
    void SetId(int newId) { id = newId; }
    
    std::string GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must inherit from Component");

        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        ptr->SetOwner(this);

        components.push_back(std::move(comp));
        ptr->Start();

        return ptr;
    }

    template<typename T>
    T* GetComponent() {
        for (auto& c : components) {
            if (T* casted = dynamic_cast<T*>(c.get()))
                return casted;
        }
        return nullptr;
    }
    
    template<typename T>
    std::vector<T*> GetComponents() {
        std::vector<T*> result;
        for (auto& c : components) {
            if (T* casted = dynamic_cast<T*>(c.get()))
                result.push_back(casted);
        }
        return result;
    }

    virtual void Update(float dt) {
        for (auto& c : components)
            c->Update(dt);
    }
    
    virtual void Start() {}
};
