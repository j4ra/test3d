#pragma once
#include "App.h"
#include "Timer.h"

namespace Application {
    class Time {
    public:
        inline Time() : totalTimer(), deltaTimer()
        {}

        /// <summary>
        /// Returns time since app loaded.
        /// </summary>
        /// <returns></returns>
        inline float TotalTime() const {
            return totalTime_;
        }

        /// <summary>
        /// Returns time since last update.
        /// </summary>
        /// <returns></returns>
        inline float DeltaTime() const {
            return deltaTime_;
        }

        inline void Update() {
            deltaTime_ = deltaTimer.Peek();
            totalTime_ = totalTimer.Peek();

            deltaTimer.Mark();
        }
    private:
        float totalTime_ = 0.0f;
        float deltaTime_ = 0.0f;

        Utils::Timer totalTimer;
        Utils::Timer deltaTimer;
    };
}