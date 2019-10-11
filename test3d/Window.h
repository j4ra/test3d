#pragma once

#include "MiniWindows.h"
#include "BaseException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <string>
#include <memory>

#define WND_CLASS_NAME "test3d window"

class Window 
{
public:
    class Exception : public BaseException
    {
    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        virtual const char* GetType() const noexcept;
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
    private:
        HRESULT hr;
    };
private:
    class WindowClass
    {
    public:
        const char* GetName() const noexcept;
        HINSTANCE GetInstance() const noexcept;
        WindowClass(const char* name);
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
    private:
        std::string wndClassName;
        HINSTANCE hInst;
    };

public:
    Window(int width, int height, const char* name);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Graphics& Gfx();
    void SetTitle(const std::string& title);
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
    Keyboard kbd;
    Mouse mouse;
private:
    WindowClass wndClass;
    int width;
    int height;
    HWND hWnd;
    std::unique_ptr<Graphics> pGfx;
};

#define BWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define BWND_LAST_ERROR() Window::Exception(__LINE__, __FILE__, GetLastError())