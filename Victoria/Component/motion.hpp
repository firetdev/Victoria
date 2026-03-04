#pragma once

#include "component.hpp"
#include "../Entity/entity.hpp"
#include "../Core/vec2.hpp"

class Motion : public Component {
public:
    Vec2 velocity;
    float retain = 0.0;
    
    void Update(float dt) override {
        auto transform = owner->GetComponent<Transform>();
        if (!transform) return;
        
        transform->position.x += velocity.x * dt;
        transform->position.y += velocity.y * dt;
        
        velocity *= std::pow(retain, dt);
        
        if (abs(velocity.x) < 0.01)
            velocity.x = 0.0;
        if (abs(velocity.y) < 0.01)
            velocity.y = 0.0;
    }
};
