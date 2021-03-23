#include "App.h"
#include "RenderComponent.h"
#include <sstream>

namespace Application {
    App::App() : wnd(800, 600, "Window0"), time()
    {
        Rendering::Shaders::ShaderBank::InitializeShaders(wnd.Gfx());

        gameObjects.push_back(std::make_unique<Rendering::RenderComponent>());
        for (auto& go : gameObjects) {
            go->Start(time, wnd.Gfx());
        }

    }

    int App::Run()
    {
        while (true)
        {
            if (const auto ecode = Window::ProcessMessages())
            {
                return *ecode;
            }
            Update();
        }
    }

    void App::Update()
    {
        time.Update();

        if (frameTimes.size() >= 16)
            frameTimes.pop_back();
        frameTimes.push_front(time.DeltaTime());

        float total = 0.0f;
        for (auto& dt : frameTimes) {
            total += dt;
        }
        total /= 16.0f;

        std::ostringstream oss;
        oss << "delta: " << total * 1000.0f << "ms";
        wnd.SetTitle(oss.str());

        const float t = time.TotalTime();

        float r = 0.5f * (std::sin(t) + 1);
        r *= r;
        float g = std::sin(2 * t);
        g *= g;
        float b = std::sin(3 * t);
        b *= b;
        wnd.Gfx().ClearBuffer(r, g, b);
        GameObject::Input input = { wnd.GetMouse(), wnd.GetKeyboard() };

        for (auto& go : gameObjects) {
            go->Update(time, wnd.Gfx(), input);
        }

        //wnd.Gfx().DrawTestTriangle(t, wnd.GetMouse().GetPosX() / 400.0f - 1.0f, 1.0f - wnd.GetMouse().GetPosY() / 300.0f);
        wnd.Gfx().EndFrame();
    }
}