// Engine include
#pragma once

#include "Component/components.hpp"
#include "Entity/entity.hpp"
#include "Core/vec2.hpp"
#include "Core/input.hpp"
#include "Scene/scene.hpp"

#include <map>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

namespace Victoria {
inline std::string pendingSceneChange = "";

// Scene registry
inline std::map<std::string, std::function<std::shared_ptr<Scene>()>> registry;

inline std::shared_ptr<Scene> currentScene = nullptr;

inline void RegisterScene(const std::string& name, std::function<std::shared_ptr<Scene>()> creator) {
    registry[name] = creator;
}

inline void ChangeScene(const std::string& name) {
    auto it = registry.find(name);
    if (it != registry.end()) {
        currentScene = it->second();  // Create new scene instance
        currentScene->Start();
    } else {
        std::cerr << "[SceneManager] Scene \"" << name << "\" not found!\n";
    }
}

inline void QueueSceneChange(const std::string& name) {
    pendingSceneChange = name;
}

inline void Update(float dt) {
    if (currentScene)
        currentScene->Update(dt);
    
    if (!pendingSceneChange.empty()) {
        ChangeScene(pendingSceneChange);
        pendingSceneChange = "";
    }
}

inline std::shared_ptr<Scene> GetCurrentScene() {
    return currentScene;
}
}  // End namespace
