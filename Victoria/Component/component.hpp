#pragma once

#include <memory>

class Entity;  // Forward declaration

class Component {
protected:
    Entity* owner = nullptr;

public:
    virtual ~Component() = default;

    void SetOwner(Entity* o) { owner = o; }

    virtual void Start() {}
    virtual void Update(float dt) {}
};
