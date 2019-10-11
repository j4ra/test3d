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

    float r = std::sin(t);
    r *= r;
    float g = std::sin(2 * t);
    g *= g;
    float b = std::sin(3 * t);
    b *= b;

    wnd.Gfx().ClearBuffer(r, g, b);
    wnd.Gfx().DrawTestTriangle();
    wnd.Gfx().EndFrame();
}
