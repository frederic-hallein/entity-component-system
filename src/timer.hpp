#ifndef TIMER_HPP
#define TIMER_HPP

#include "pch.hpp"

class Timer {
public:
    Timer() : mDeltaTime(0.0f) {}

    void start() {
        mStartTime = std::chrono::high_resolution_clock::now();
    }

    void end() {
        auto endTime = std::chrono::high_resolution_clock::now();
        mDeltaTime = std::chrono::duration<float>(endTime - mStartTime).count();
    }

    f32 getDeltaTime() const { return mDeltaTime; }
    f32 getFPS() const { return mDeltaTime > 0.0f ? 1.0f / mDeltaTime : 0.0f; }

private:
    std::chrono::high_resolution_clock::time_point mStartTime;
    f32 mDeltaTime;
};

#endif