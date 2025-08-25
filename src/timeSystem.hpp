#ifndef TIME_SYSTEM_HPP
#define TIME_SYSTEM_HPP

#include "system.hpp"

namespace ecs {

    template<u64 MaxEntities>
    class TimeSystem : public System<MaxEntities> {
    public:
        TimeSystem() : mLastTime(std::chrono::high_resolution_clock::now()), mDeltaTime(0.0f) {}

        void update(f32 deltaTime) override {
            auto currentTime = std::chrono::high_resolution_clock::now();
            mDeltaTime = std::chrono::duration<float>(currentTime - mLastTime).count();
            mLastTime = currentTime;

            f32 fps = 1.0f / mDeltaTime;
            LOG_INFO("FPS: ", fps, ", Delta: ", mDeltaTime * 1000.0f, "ms");
        }

        f32 getDeltaTime() const { return mDeltaTime; }

    private:
        std::chrono::high_resolution_clock::time_point mLastTime;
        f32 mDeltaTime;
    };

}

#endif