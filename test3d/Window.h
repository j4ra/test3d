#pragma once

#include "MiniWindows.h"
#include "BaseException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <string>
#include <memory>
#include <optional>

#define WND_CLASS_NAME "test3d_window"

namespace Application {
    class Window
    {
    public:
        class Exception : public BaseException
        {
            using BaseException::BaseException;
        public:
            static std::string TranslateMessage(HRESULT hr);
        };
        class HrException : public Exception
        {
            using Exception::Exception;
        public:
            HrException(int line, const char* file, HRESULT hr) noexcept;
            std::string GetErrorString() const noexcept;
            HRESULT GetErrorCode() const noexcept;
            const char* GetType() const noexcept override;
            const char* what() const noexcept override;
        private:
            HRESULT hr;

        };
        class NoGfxException : public Exception
        {
            using Exception::Exception;
        public:
            const char* GetType() const noexcept override;
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
        inline const Mouse& GetMouse() { return mouse; }
        inline const Keyboard& GetKeyboard() { return kbd; }
        void SetTitle(const std::string& title);
        static std::optional<int> ProcessMessages();
    private:
        static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    private:
        WindowClass wndClass;
        Keyboard kbd;
        Mouse mouse;
        int width;
        int height;
        HWND hWnd;
        std::unique_ptr<Graphics> pGfx;
    };
}