#include "pch.hpp"

#include "engine.hpp"

constexpr const char ECS_TITLE[] = "ECS";
constexpr u64 MAX_ENTITIES = 10;
constexpr u16 WINDOW_WIDTH = 1080;
constexpr u16 WINDOW_HEIGHT = 720;

using namespace ecs;

int main() {
    Engine<ECS_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, MAX_ENTITIES> engine;
    while (!engine.shouldClose()) {
        engine.run();
    }

    engine.shutdown();
    return 0;
}