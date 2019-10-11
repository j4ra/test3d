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

    float a = 0.5 * (std::sin(t) + 1.0);
    wnd.Gfx().ClearBuffer(a, a, 1.0);

    wnd.Gfx().EndFrame();
}
