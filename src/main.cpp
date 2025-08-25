#include "pch.hpp"

#include "entityManager.hpp"
#include "componentManager.hpp"
#include "systemManager.hpp"
#include "systems.hpp"
#include "windowSystem.hpp"

constexpr u64 MAX_ENTITIES = 1000000;
constexpr u16 WINDOW_WIDTH = 1080;
constexpr u16 WINDOW_HEIGHT = 720;

using namespace ecs;

int main() {
    printInfo();

    EntityManager<MAX_ENTITIES> entityManager;
    ComponentManager<MAX_ENTITIES> componentManager;
    SystemManager<MAX_ENTITIES> systemManager(&componentManager);

    // Register systems
    systemManager.registerSystem<WindowSystem<MAX_ENTITIES, WINDOW_WIDTH, WINDOW_HEIGHT>>();
    systemManager.registerSystem<MovementSystem<MAX_ENTITIES>>();
    systemManager.registerSystem<HealthSystem<MAX_ENTITIES>>();
    systemManager.registerSystem<RenderSystem<MAX_ENTITIES>>();

    Position pos{100, 200};
    Position pos2{130, 560};
    Velocity vel{0, -10};

    u64 playerId = 1;
    componentManager.setComponent(playerId, pos);
    componentManager.setComponent(playerId, pos2); //warn
    componentManager.setComponent(playerId, vel);

    // Game loop
    float deltaTime = 0.016f; // ~60 FPS
    auto lastTime = std::chrono::high_resolution_clock::now();

    // auto* windowSystem = systemManager.getSystem<WindowSystem<MAX_ENTITIES>>();
    auto* windowSystem = systemManager.getSystem<WindowSystem<MAX_ENTITIES, WINDOW_WIDTH, WINDOW_HEIGHT>>();
    while (!windowSystem->shouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Update all systems
        systemManager.updateSystems(deltaTime);
    }

    systemManager.shutdown();
    return 0;
}