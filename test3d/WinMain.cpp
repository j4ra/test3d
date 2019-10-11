#include "MiniWindows.h"
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, 
                     HINSTANCE hPrevInstance, 
                     LPSTR lpCmdLine, 
                     int cmdShow)
{
    try
    {
    Window wnd(800, 600, "Window0");

    MSG msg;
    BOOL gResult;


        while (gResult = GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            wnd.Gfx().EndFrame();
        }
        if (gResult == -1)
        {
            return -1;
        }

        return msg.wParam;
    }
    catch (const BaseException& e)
    {
        MessageBox(NULL, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(NULL, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(NULL, "No details avialable!", "Unknown", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}