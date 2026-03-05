#pragma once

#include "../Entity/entity.hpp"

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

class Scene {
protected:
    std::string name;
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Entity>> entities;

public:
    Scene(std::string n, sf::RenderWindow& win) : name(n), window(win) {}
    ~Scene() = default;

    Entity* AddEntity(std::unique_ptr<Entity> entity) {
        Entity* ptr = entity.get();
        entities.push_back(std::move(entity));
        return ptr;
    }

    void RemoveEntity(Entity* entity) {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                           [entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; }),
            entities.end()
        );
    }
    
    void RemoveEntityByName(const std::string& name) {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                           [&name](const std::unique_ptr<Entity>& e) { return e->GetName() == name; }),
            entities.end()
        );
    }
    
    Entity* GetEntityByName(const std::string& searchName) {
        for (auto& e : entities) {
            if (e->GetName() == searchName)
                return e.get();
        }
        return nullptr;
    }
    
    std::string GetName() const { return name; }

    virtual void Update(float dt) {
        for (auto& e : entities)
            e->Update(dt);
    }
    
    virtual void Start() {}
};
