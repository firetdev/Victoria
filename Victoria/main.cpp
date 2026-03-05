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
        auto tCollider = Victoria::GetCurrentScene()->GetEntity("target")->GetComponent<CollisionBox>();
        if (pCollider->IsColliding(tCollider))
            owner->GetComponent<Transform>()->position += pCollider->GetPenetration(tCollider);
    }
};

class Player : public Entity {
public:
    Player(std::string name) : Entity(name) {}
    
    void Start() override {
        AddComponent<Transform>();
        AddComponent<Motion>();
        GetComponent<Motion>()->retain = 0.2;
        AddComponent<CollisionBox>();
        AddComponent<PlayerScript>();
        AddComponent<Sprite>("player.png", Victoria::GetWindow());
        AddComponent<Camera>(Victoria::GetWindow());
        if (Victoria::GetCurrentScene()->GetName() != "MainScene2")
            GetComponent<Camera>()->SetOffset({32, 32});
        else
            GetComponent<Camera>()->SetOffset({64, 64});
        
        auto pCollider = GetComponent<CollisionBox>();
        pCollider->position = {0, 0};
        pCollider->size = {64, 64};
    }
};

class Target : public Entity {
public:
    Target(std::string name) : Entity(name) {}
    
    void Start() override {
        AddComponent<Transform>();
        GetComponent<Transform>()->position = {200, 200};
        AddComponent<CollisionBox>();
        
        auto tCollider = GetComponent<CollisionBox>();
        tCollider->position = {0, 0};
        tCollider->size = {64, 64};
        AddComponent<Sprite>("player.png", Victoria::GetWindow());
    }
};

class MainScene : public Scene {
public:
    MainScene(std::string n, sf::RenderWindow& win) : Scene(n, win) {}
    
    void Start() override {
        AddEntity(std::make_unique<Player>("player"));
        AddEntity(std::make_unique<Target>("target"));
    }
};

class MainScene2 : public Scene {
public:
    MainScene2(std::string n, sf::RenderWindow& win) : Scene(n, win) {}
    
    void Start() override {
        AddEntity(std::make_unique<Player>("player"));
        AddEntity(std::make_unique<Target>("target"));
        
        GetEntity("player")->GetComponent<CollisionBox>()->size = {128, 128};
        GetEntity("player")->GetComponent<Sprite>()->scale = {2, 2};
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
