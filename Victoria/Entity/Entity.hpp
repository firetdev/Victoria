#pragma once

#include <vector>
#include <memory>
#include <type_traits>

class Component;  // Forward declaraction

class Entity {
protected:
    std::vector<std::unique_ptr<Component>> components;

public:
    virtual ~Entity() = default;

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
};
