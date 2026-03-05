#include "victoria.hpp"

class PlayerScript : public Component {
protected:
    void Update(float dt) override {
        auto motion = owner->GetComponent<Motion>();
        
        if (Input::inputs["w"]) motion->velocity.y = -150;
        if (Input::inputs["s"]) motion->velocity.y = 150;
        if (Input::inputs["a"]) motion->velocity.x = -150;
        if (Input::inputs["d"]) motion->velocity.x = 150;
        
        if (Input::inputs["c"])
            Victoria::QueueSceneChange("MainScene2");
        
        auto pCollider = owner->GetComponent<CollisionBox>();
        auto tCollider = Victoria::GetCurrentScene()->GetEntityByName("target")->GetComponent<CollisionBox>();
        if (pCollider->IsColliding(*tCollider))
            owner->GetComponent<Transform>()->position += pCollider->GetPenetration(*tCollider);
    }
};

class MainScene : public Scene {
public:
    MainScene(std::string n, sf::RenderWindow& win) : Scene(n, win) {}
    
    Entity* player = nullptr;
    Entity* target = nullptr;
    CollisionBox* pCollider = nullptr;
    CollisionBox* tCollider = nullptr;
    
    void Start() override {
        player = AddEntity(std::make_unique<Entity>("player"));
        target = AddEntity(std::make_unique<Entity>("target"));
        
        player->AddComponent<Transform>();
        player->AddComponent<Motion>();
        player->GetComponent<Motion>()->retain = 0.2;
        player->AddComponent<CollisionBox>();
        player->AddComponent<PlayerScript>();
        
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
};

class MainScene2 : public Scene {
public:
    MainScene2(std::string n, sf::RenderWindow& win) : Scene(n, win) {}
    
    Entity* player = nullptr;
    Entity* target = nullptr;
    CollisionBox* pCollider = nullptr;
    CollisionBox* tCollider = nullptr;
    
    void Start() override {
        player = AddEntity(std::make_unique<Entity>("player"));
        target = AddEntity(std::make_unique<Entity>("target"));

        player->AddComponent<Transform>();
        player->AddComponent<Motion>();
        player->GetComponent<Motion>()->retain = 0.2;
        player->AddComponent<CollisionBox>();
        player->AddComponent<PlayerScript>();
        
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
};

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Victoria demo game");
    sf::Clock clock;
    
    Victoria::RegisterScene([&window]() -> std::shared_ptr<Scene> {
        return std::make_shared<MainScene>("MainScene", window);
    });
    Victoria::RegisterScene([&window]() -> std::shared_ptr<Scene> {
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
