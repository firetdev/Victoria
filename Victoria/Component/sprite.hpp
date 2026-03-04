#pragma once

#include "component.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>
#include <iostream>

class Transform;  // Forward declaration

class Sprite : public Component {
private:
    std::unique_ptr<sf::Sprite> sprite;
    std::shared_ptr<sf::Texture> texture;
    std::shared_ptr<sf::Shader> shader;
    sf::RenderWindow& window;

public:
    Vec2 position = {0.f, 0.f};
    Vec2 scale = {1.f, 1.f};
    float rotation = 0.f;
    bool visible = true;

    Sprite(const std::string& filepath, sf::RenderWindow& win)
        : sprite(nullptr), texture(nullptr), window(win)
    {
        if (!Load(filepath))
            std::cout << "Sprite failed to initialize: " << filepath << std::endl;
    }

    bool Load(const std::string& filepath) {
        texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(filepath))
            return false;

        sprite = std::make_unique<sf::Sprite>(*texture);

        ApplyProperties();
        return true;
    }
    
    bool LoadShader(const std::string& shaderPath, sf::Shader::Type type = sf::Shader::Type::Fragment) {
        shader = std::make_shared<sf::Shader>();
        if (!shader->loadFromFile(shaderPath, type)) {
            shader = nullptr;
            return false;
        }
        return true;
    }

    void ApplyProperties() {
        if (sprite) {
            sprite->setPosition({position.x, position.y});
            sprite->setScale({scale.x, scale.y});
            sprite->setRotation(sf::degrees(rotation));
        }
    }
    
    void Scale(const Vec2& s) {
        scale.x *= s.x;
        scale.y *= s.y;
        if (sprite) sprite->setScale({scale.x, scale.y});
    }

    void Rotate(float r) {
        rotation += r;
        if (sprite) sprite->setRotation(sf::degrees(rotation));
    }

    void Move(float dx, float dy) {
        position.x += dx;
        position.y += dy;
        if (sprite) sprite->setPosition({position.x, position.y});
    }

    void SetPosition(const Vec2& pos) {
        position = pos;
        if (sprite) sprite->setPosition({position.x, position.y});
    }

    void SetScale(const Vec2& s) {
        scale = s;
        if (sprite) sprite->setScale({scale.x, scale.y});
    }

    void SetRotation(float r) {
        rotation = r;
        if (sprite) sprite->setRotation(sf::degrees(rotation));
    }

    sf::Sprite* GetSprite() { return sprite.get(); }

    void Update(float dt) override {
        if (!visible || !sprite) return;

        ApplyProperties();
        Transform* t = owner->GetComponent<Transform>();
        if (t) {
            sprite->setPosition({position.x + t->position.x, position.y + t->position.y});
            sprite->setScale({scale.x * t->scale.x, scale.y * t->scale.y});
        }

        if (shader)
            window.draw(*sprite, shader.get());
        else
            window.draw(*sprite);
    }
};
