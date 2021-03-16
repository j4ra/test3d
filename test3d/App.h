#pragma once

#include "Window.h"
#include "Time.h"
#include "GameObject.h"
#include <vector>

namespace Application {
    class App
    {
    public:
        using GameObjectPtr = std::unique_ptr<GameObject>;
    public:
        App();
        int Run();
    private:
        void Update();
    private:
        Window wnd;
        Time time;
        std::vector<GameObjectPtr> gameObjects;
        std::deque<float> frameTimes;
    };
}