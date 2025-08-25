#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "entityManager.hpp"
#include "componentManager.hpp"
#include "systemManager.hpp"
#include "timeSystem.hpp"
#include "windowSystem.hpp"
#include "systems.hpp"

namespace ecs {

template<cstr Title, u16 WindowWidth, u16 WindowHeight, u64 MaxEntities>
    class Engine {
    public:
        Engine() {
            printInfo();

            // Create ECS managers - Engine owns them
            mEntityManager = std::make_unique<EntityManager<MaxEntities>>();
            mComponentManager = std::make_unique<ComponentManager<MaxEntities>>();
            mSystemManager = std::make_unique<SystemManager<MaxEntities>>(mComponentManager.get());

            // Register systems
            mSystemManager->template registerSystem<WindowSystem<Title, WindowWidth, WindowHeight, MaxEntities>>();
            mSystemManager->template registerSystem<TimeSystem<MaxEntities>>();
            mSystemManager->template registerSystem<MovementSystem<MaxEntities>>();
            mSystemManager->template registerSystem<HealthSystem<MaxEntities>>();
            mSystemManager->template registerSystem<RenderSystem<MaxEntities>>();

            // Get system references
            mTimeSystem = mSystemManager->template getSystem<TimeSystem<MaxEntities>>();
            mWindowSystem = mSystemManager->template getSystem<WindowSystem<Title, WindowWidth, WindowHeight, MaxEntities>>();

            // Setup test entities
            u64 playerId = 1;
            Position pos{100, 200};
            Velocity vel{0, -10};
            mComponentManager->setComponent(playerId, pos);
            mComponentManager->setComponent(playerId, vel);

            LOG_INFO("Engine initialized");
        }

        void run() {
            mTimeSystem->update(0.0f);
            float deltaTime = mTimeSystem->getDeltaTime();
            mSystemManager->updateSystems(deltaTime);
        }

        // Check if window should close
        bool shouldClose() const {
            return mWindowSystem->shouldClose();
        }

        // Shutdown method
        void shutdown() {
            LOG_INFO("Shutting down engine...");
            mSystemManager->shutdown();
        }

    private:
        std::unique_ptr<EntityManager<MaxEntities>> mEntityManager;
        std::unique_ptr<ComponentManager<MaxEntities>> mComponentManager;
        std::unique_ptr<SystemManager<MaxEntities>> mSystemManager;
        TimeSystem<MaxEntities>* mTimeSystem;
        WindowSystem<Title, WindowWidth, WindowHeight, MaxEntities>* mWindowSystem;
    };

}

#endif