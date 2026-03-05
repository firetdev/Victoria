#pragma once

#include "component.hpp"
#include "transform.hpp"
#include "../Core/vec2.hpp"

#include <algorithm>

class CollisionBox : public Component {
public:
    // Local position and size
    Vec2 position{0, 0};
    Vec2 size{1, 1};

    // For potential future use in engine or games
    bool isPhysical = false;

public:
    CollisionBox() = default;
    CollisionBox(const Vec2& pos, const Vec2& s)
        : position(pos), size(s) {}

    Vec2 GetWorldPosition() const {
        Transform* transform = owner->GetComponent<Transform>();
        if (!transform) return position;

        return {
            transform->position.x + position.x,
            transform->position.y + position.y
        };
    }

    Vec2 GetWorldSize() const {
        Transform* transform = owner->GetComponent<Transform>();
        if (!transform) return size;

        return {
            size.x * transform->scale.x,
            size.y * transform->scale.y
        };
    }

    bool IsColliding(const CollisionBox* other) const {
        Vec2 aPos = GetWorldPosition();
        Vec2 aSize = GetWorldSize();

        Vec2 bPos = other->GetWorldPosition();
        Vec2 bSize = other->GetWorldSize();

        return (
            aPos.x < bPos.x + bSize.x &&
            aPos.x + aSize.x > bPos.x &&
            aPos.y < bPos.y + bSize.y &&
            aPos.y + aSize.y > bPos.y
        );
    }
    
    Vec2 GetPenetration(const CollisionBox* other) const {
        Vec2 aPos = GetWorldPosition();
        Vec2 aSize = GetWorldSize();

        Vec2 bPos = other->GetWorldPosition();
        Vec2 bSize = other->GetWorldSize();

        // Compute edges
        float aLeft = aPos.x;
        float aRight = aPos.x + aSize.x;
        float aTop = aPos.y;
        float aBottom = aPos.y + aSize.y;

        float bLeft = bPos.x;
        float bRight = bPos.x + bSize.x;
        float bTop = bPos.y;
        float bBottom = bPos.y + bSize.y;

        // Compute overlap on both axes
        float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
        float overlapY = std::min(aBottom, bBottom) - std::max(aTop, bTop);

        // If no collision, return zero
        if (overlapX <= 0 || overlapY <= 0)
            return {0, 0};

        // Resolve along the smaller axis (minimum translation vector)
        if (overlapX < overlapY) {
            if (aLeft < bLeft)
                return {-overlapX, 0};  // Push left
            else
                return {overlapX, 0};  // Push right
        } else {
            if (aTop < bTop)
                return {0, -overlapY};  // Push up
            else
                return {0, overlapY};  // Push down
        }
    }
};
