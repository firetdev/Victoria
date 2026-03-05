#include "victoria.hpp"

class MainScene : public Scene {
public:
    MainScene(std::string n, sf::RenderWindow& win) : Scene(n, win) {}
    
    Entity* player = nullptr;
    Entity* target = nullptr;
    CollisionBox* pCollider = nullptr;
    CollisionBox* tCollider = nullptr;
    
    void Start() override {
        player = AddEntity(std::make_unique<Entity>());
        target = AddEntity(std::make_unique<Entity>());
        
        player->AddComponent<Transform>();
        player->AddComponent<Motion>();
        player->GetComponent<Motion>()->retain = 0.2;
        player->AddComponent<CollisionBox>();
        
        pCollider = player->GetComponent<CollisionBox>();
        pCollider->position = {0, 0};
        pCollider->size = {64, 64};
        player->AddComponent<Sprite>("player.png", window);
        
        target->AddComponent<Transform>();
        target->GetComponent<Transform>()->position = {200, 200};
        target->AddComponent<CollisionBox>();
        tCollider = target->GetComponent<CollisionBox>();
        tCollider->position = {0, 0};
        tCollider->size = {64, 64};
        target->AddComponent<Sprite>("player.png", window);
    }
    
    void Update(float dt) override {
        for (auto& e : entities)
            e->Update(dt);
        
        auto playerMotion = player->GetComponent<Motion>();
        
        if (Input::inputs["w"]) playerMotion->velocity.y = -150;
        if (Input::inputs["s"]) playerMotion->velocity.y = 150;
        if (Input::inputs["a"]) playerMotion->velocity.x = -150;
        if (Input::inputs["d"]) playerMotion->velocity.x = 150;
        
        if (Input::inputs["c"])
            Victoria::QueueSceneChange("Main2");
        
        if (pCollider->IsColliding(*tCollider)) {
            player->GetComponent<Transform>()->position += pCollider->GetPenetration(*tCollider);
        }
    }
};

class MainScene2 : public Scene {
public:
    MainScene2(std::string n, sf::RenderWindow& win) : Scene(n, win) {}
    
    Entity* player = nullptr;
    Entity* target = nullptr;
    CollisionBox* pCollider = nullptr;
    CollisionBox* tCollider = nullptr;
    
    void Start() override {
        player = AddEntity(std::make_unique<Entity>());
        target = AddEntity(std::make_unique<Entity>());

        player->AddComponent<Transform>();
        player->AddComponent<Motion>();
        player->GetComponent<Motion>()->retain = 0.2;
        player->AddComponent<CollisionBox>();
        pCollider = player->GetComponent<CollisionBox>();
        pCollider->position = {0, 0};
        pCollider->size = {128, 128};
        player->AddComponent<Sprite>("player.png", window);
        player->GetComponent<Sprite>()->SetScale({2.0, 2.0});
        
        target->AddComponent<Transform>();
        target->GetComponent<Transform>()->position = {200, 200};
        target->AddComponent<CollisionBox>();
        tCollider = target->GetComponent<CollisionBox>();
        tCollider->position = {0, 0};
        tCollider->size = {64, 64};
        target->AddComponent<Sprite>("player.png", window);
    }
    
    void Update(float dt) override {
        for (auto& e : entities)
            e->Update(dt);
        
        auto playerMotion = player->GetComponent<Motion>();
        
        if (Input::inputs["w"]) playerMotion->velocity.y = -150;
        if (Input::inputs["s"]) playerMotion->velocity.y = 150;
        if (Input::inputs["a"]) playerMotion->velocity.x = -150;
        if (Input::inputs["d"]) playerMotion->velocity.x = 150;
        
        if (pCollider->IsColliding(*tCollider))
            player->GetComponent<Transform>()->position += pCollider->GetPenetration(*tCollider);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "SFML works!");
    sf::Clock clock;
    
    Victoria::RegisterScene("MainScene", [&window]() -> std::shared_ptr<Scene> {
        return std::make_shared<MainScene>("MainScene", window);
    });
    Victoria::RegisterScene("Main2", [&window]() -> std::shared_ptr<Scene> {
        return std::make_shared<MainScene2>("MainScene2", window);
    });
    
    Victoria::ChangeScene("MainScene");

    while (window.isOpen()) {
        Input::update(window);
        
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        window.clear();
        Victoria::Update(deltaTime);
        window.display();
    }
    
    Victoria::currentScene.reset();
    return 0;
}
