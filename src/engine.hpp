#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "world.hpp"
#include "timer.hpp"
#include "window.hpp"

namespace ecs {
    class Engine {
    static constexpr u64 MAX_ENTITIES = 10;
    public:

        // void loadEntities(const str& filename) {
        //     if (!std::ifstream(filename)) {
        //         LOG_ERROR("Could not open entity file: ", filename);
        //         return;
        //     }

        //     YAML::Node entities = YAML::LoadFile(filename);
        //     for (const auto& entityNode : entities) {
        //         u64 id = entityNode["id"].as<u64>();
        //         auto components = entityNode["components"];
        //         if (!components) {
        //             LOG_DEBUG("ID = ", id, " does not have components.");
        //             return;
        //         }

        //         // if (components["Position"]) {
        //         //     auto pos = components["Position"];
        //         //     mComponentManager->setComponent(id, Position{pos["x"].as<f32>(), pos["y"].as<f32>()});
        //         // }
        //         // if (components["Velocity"]) {
        //         //     auto vel = components["Velocity"];
        //         //     mComponentManager->setComponent(id, Velocity{vel["x"].as<f32>(), vel["y"].as<f32>()});
        //         // }

        //     }
        // }

        Engine(cstr title, u16 width, u16 height) {
            printInfo();

            // Create window and timer
            mWindow = std::make_unique<Window>(title, width, height);
            mTimer = std::make_unique<Timer>();

            // // Setup entities
            // loadEntities("../data/entities.yaml");

            // Init ECS
            mWorld = std::make_unique<World>();
            mWorld->createEntities(MAX_ENTITIES);

            Entity player = mWorld->getAllEntities()[0];
            Position playerPos{1.0f, 2.0f, 3.0f};
            Acceleration playerAcc{0.0f, 0.0f, -1.0f};
            mWorld->addComponents(player,
                playerPos,
                playerAcc
            );


            LOG_INFO("Engine initialized");
        }

        void run() {
            // Rendering
            mTimer->start();
            mWindow->render();
            mTimer->end();

            f32 deltaTime = mTimer->getDeltaTime();
            f32 fps = mTimer->getFPS();
            mWindow->setStats(deltaTime, fps);

            // Updating
        }

        bool shouldClose() const {
            return mWindow->shouldClose();
        }

        void shutdown() {
            LOG_INFO("Shutting down engine...");
            mWindow->shutdown();
        }

    private:
        std::unique_ptr<Window> mWindow;
        std::unique_ptr<Timer> mTimer;

        std::unique_ptr<World> mWorld;
    };
}

#endif