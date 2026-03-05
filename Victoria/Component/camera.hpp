#pragma once

#include "component.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

class Transform;  // forward declaration

class Camera : public Component {
private:
    sf::View view;
    sf::RenderWindow& window;

public:
    Vec2 size = {800.f, 600.f};
    Vec2 center = {0.f, 0.f};
    Vec2 offset = {0, 0};
    float rotation = 0.f;
    float zoom = 1.f;

    bool followOwner = true;

    Camera(sf::RenderWindow& win)
        : window(win), size({static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y)})
    {
        view.setSize({size.x, size.y});
        view.setCenter({center.x, center.y});
    }

    void ApplyProperties() {
        view.setSize({size.x * zoom, size.y * zoom});
        view.setCenter({center.x, center.y});
        view.setRotation(sf::degrees(rotation));
    }

    void SetOffset(const Vec2& c) {
        offset = c;
        view.setCenter({center.x, center.y});
    }

    void SetZoom(float z) {
        zoom = z;
        view.setSize({size.x * zoom, size.y * zoom});
    }

    void Zoom(float factor) {
        zoom *= factor;
        view.zoom(factor);
    }

    void Rotate(float r) {
        rotation += r;
        view.setRotation(sf::degrees(rotation));
    }
    
    void Move(float dx, float dy) {
        center.x += dx;
        center.y += dy;
        view.move({dx, dy});
    }

    sf::View& GetView() { return view; }

    void Update(float dt) override {
        if (followOwner) {
            Transform* t = owner->GetComponent<Transform>();
            if (t)
                center = t->position + offset;
        }

        ApplyProperties();
        window.setView(view);
    }
};
