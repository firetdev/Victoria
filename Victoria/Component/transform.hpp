#pragma once

#include "component.hpp"
#include "../Core/vec2.hpp"

class TransformComponent : public Component {
public:
    Vec2 position;
    Vec2 scale;
};
