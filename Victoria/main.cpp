#include <SFML/Graphics.hpp>
#include "victoria.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML works!");
    sf::Clock clock;
    
    Entity player;
    player.AddComponent<TransformComponent>();
    player.AddComponent<MovementComponent>();
    player.GetComponent<MovementComponent>()->retain = 0.2;
    
    sf::CircleShape shape(60.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        Input::update(window);
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        auto playerMovement = player.GetComponent<MovementComponent>();
        
        if (Input::inputs["w"])
            playerMovement->velocity.y = -150;
        if (Input::inputs["s"])
            playerMovement->velocity.y = 150;
        if (Input::inputs["a"])
            playerMovement->velocity.x = -150;
        if (Input::inputs["d"])
            playerMovement->velocity.x = 150;
        
        // Get delta time
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        
        player.Update(deltaTime);
        
        shape.setPosition(sf::Vector2f(player.GetComponent<TransformComponent>()->position.x, player.GetComponent<TransformComponent>()->position.y));

        window.clear();
        window.draw(shape);
        window.display();
    }
}
