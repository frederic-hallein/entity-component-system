#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "entityManager.hpp"
#include "componentManager.hpp"
#include "systemManager.hpp"
#include "timer.hpp"
#include "window.hpp"
#include "systems.hpp"

namespace ecs {
    class Engine {
        // Amount entities
        static constexpr u64 MAX_ENTITIES = 10;

        public:
        Engine(cstr title, u16 width, u16 height) {
            printInfo();

            // Create window and timer
            mWindow = std::make_unique<Window>(title, width, height);
            mTimer = std::make_unique<Timer>();


            // Create ECS managers
            mEntityManager = std::make_unique<EntityManager<MAX_ENTITIES>>();
            mComponentManager = std::make_unique<ComponentManager<MAX_ENTITIES>>();
            mSystemManager = std::make_unique<SystemManager<MAX_ENTITIES>>(mComponentManager.get());

            // Register systems
            mSystemManager->template registerSystem<MovementSystem<MAX_ENTITIES>>();
            mSystemManager->template registerSystem<HealthSystem<MAX_ENTITIES>>();
            mSystemManager->template registerSystem<RenderSystem<MAX_ENTITIES>>();

            // Setup test entities (TODO: move to some file format)
            u64 playerId = 1;
            Position pos{100, 200};
            Velocity vel{0, -10};
            mComponentManager->setComponent(playerId, pos);
            mComponentManager->setComponent(playerId, vel);

            LOG_INFO("Engine initialized");
        }

        void run() {
            mTimer->start();
            mWindow->render();
            mTimer->end();

            f32 deltaTime = mTimer->getDeltaTime();
            mSystemManager->updateSystems(deltaTime);

            LOG_INFO("Render dt: ", mTimer->getDeltaTime() * 1000.0f, "ms");
            LOG_INFO("FPS: ", mTimer->getFPS());
        }

        bool shouldClose() const {
            return mWindow->shouldClose();
        }

        void shutdown() {
            LOG_INFO("Shutting down engine...");
            mSystemManager->shutdown();
            mWindow->shutdown();
        }

    private:
        std::unique_ptr<Window> mWindow;
        std::unique_ptr<Timer> mTimer;
        std::unique_ptr<EntityManager<MAX_ENTITIES>> mEntityManager;
        std::unique_ptr<ComponentManager<MAX_ENTITIES>> mComponentManager;
        std::unique_ptr<SystemManager<MAX_ENTITIES>> mSystemManager;
    };
}

#endif