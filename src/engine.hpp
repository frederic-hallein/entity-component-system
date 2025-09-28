#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "world.hpp"
#include "timer.hpp"
#include "window.hpp"

namespace ecs {
    class Engine {
    public:
        Engine(cstr title, u16 width, u16 height) {
            printInfo();

            // Create window and timer
            mWindow = std::make_unique<Window>(title, width, height);
            mTimer = std::make_unique<Timer>();

            // Create World
            mWorld = std::make_unique<World>();

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