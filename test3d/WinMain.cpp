#include "MiniWindows.h"
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, 
                     HINSTANCE hPrevInstance, 
                     LPSTR lpCmdLine, 
                     int cmdShow)
{
    Window wnd(800, 600, "Window0");
    
    MSG msg;
    BOOL gResult;

    while (gResult = GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (gResult == -1)
    {
        return -1;
    }

    return msg.wParam;
}