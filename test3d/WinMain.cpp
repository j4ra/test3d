#include "MiniWindows.h"
#include "App.h"
#include "BaseException.h"

int CALLBACK WinMain(HINSTANCE hInstance, 
                     HINSTANCE hPrevInstance, 
                     LPSTR lpCmdLine, 
                     int cmdShow)
{
    try
    {
        return App{}.Run();
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