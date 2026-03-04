#include "victoria.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML works!");
    sf::Clock clock;
    
    Entity player;
    player.AddComponent<Transform>();
    player.AddComponent<Motion>();
    player.GetComponent<Motion>()->retain = 0.2;
    
    sf::CircleShape shape(60.f);
    shape.setFillColor(sf::Color::Green);

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
        
        // Get delta time
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        
        player.Update(deltaTime);
        
        shape.setPosition(sf::Vector2f(player.GetComponent<Transform>()->position.x, player.GetComponent<Transform>()->position.y));

        window.clear();
        window.draw(shape);
        window.display();
    }
}
