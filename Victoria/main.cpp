#include "victoria.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML works!");
    sf::Clock clock;
    
    Entity player;
    player.AddComponent<Transform>();
    player.AddComponent<Motion>();
    player.GetComponent<Motion>()->retain = 0.2;
    player.AddComponent<CollisionBox>();
    auto pCollider = player.GetComponent<CollisionBox>();
    pCollider->position = {0, 0};
    pCollider->size = {60, 60};
    
    Entity target;
    target.AddComponent<Transform>();
    target.GetComponent<Transform>()->position = {200, 200};
    target.AddComponent<CollisionBox>();
    auto tCollider = target.GetComponent<CollisionBox>();
    tCollider->position = {0, 0};
    tCollider->size = {60, 60};
    
    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Green);
    
    sf::CircleShape tShape(30.f);
    tShape.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        Input::update(window);
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        auto playerMotion = player.GetComponent<Motion>();
        
        if (Input::inputs["w"])
            playerMotion->velocity.y = -150;
        if (Input::inputs["s"])
            playerMotion->velocity.y = 150;
        if (Input::inputs["a"])
            playerMotion->velocity.x = -150;
        if (Input::inputs["d"])
            playerMotion->velocity.x = 150;
        
        // Collision
        if (pCollider->IsColliding(*tCollider))
            player.GetComponent<Transform>()->position += pCollider->GetPenetration(*tCollider);
        
        // Get delta time
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        
        player.Update(deltaTime);
        
        shape.setPosition(sf::Vector2f(player.GetComponent<Transform>()->position.x, player.GetComponent<Transform>()->position.y));
        tShape.setPosition(sf::Vector2f(target.GetComponent<Transform>()->position.x, target.GetComponent<Transform>()->position.y));

        window.clear();
        window.draw(shape);
        window.draw(tShape);
        window.display();
    }
}
