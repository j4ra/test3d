#pragma once

#include "MiniWindows.h"

#define WND_CLASS_NAME "test3d window"

class Window 
{
private:
    class WindowClass
    {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const char* wndClassName = WND_CLASS_NAME;
        static WindowClass wndClass;
        HINSTANCE hInst;
    };
public:
    Window(int width, int height, const char* name) noexcept;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
    int width;
    int height;
    HWND hWnd;
};