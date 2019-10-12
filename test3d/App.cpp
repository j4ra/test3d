#include "App.h"

App::App() : wnd(800,600,"Window0"), time()
{}

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
    const float t = time.Peek();

    float r = 0.5f * (std::sin(t) + 1);
    r *= r;
    float g = std::sin(2 * t);
    g *= g;
    float b = std::sin(3 * t);
    b *= b;

    wnd.Gfx().ClearBuffer(r, g, b);
    wnd.Gfx().DrawTestTriangle(t, wnd.mouse.GetPosX() / 400.0f - 1.0f, 1.0f - wnd.mouse.GetPosY() / 300.0f);
    wnd.Gfx().EndFrame();
}
