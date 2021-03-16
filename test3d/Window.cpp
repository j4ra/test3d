#include "Window.h"
#include "resource.h"
#include <sstream>

#define WND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define WND_LAST_ERROR() Window::HrException(__LINE__, __FILE__, GetLastError())
#define WND_NO_GFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)

namespace Application {
    Window::WindowClass::WindowClass(const char* name)
        : hInst(GetModuleHandle(NULL)), wndClassName(WND_CLASS_NAME)
    {
        wndClassName.append(name);

        WNDCLASSEX wc = { 0 };
        wc.cbSize = sizeof(wc);
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
        wc.hCursor = NULL;
        wc.hInstance = GetInstance();
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
        wc.lpszClassName = GetName();

        if (RegisterClassEx(&wc) == 0) throw WND_LAST_ERROR();

    }

    Window::WindowClass::~WindowClass()
    {
        UnregisterClass(GetName(), hInst);
        hInst = NULL;
    }

    const char* Window::WindowClass::GetName() const noexcept
    {
        return wndClassName.c_str();
    }

    HINSTANCE Window::WindowClass::GetInstance() const noexcept
    {
        return hInst;
    }




    Window::Window(int width, int height, const char* name)
        : wndClass(name), height(height), width(width)
    {
        constexpr DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        RECT wr;
        wr.left = 100;
        wr.right = width + wr.left;
        wr.top = 100;
        wr.bottom = height + wr.top;

        if (AdjustWindowRect(&wr, style, FALSE) == 0) throw WND_LAST_ERROR();

        hWnd = CreateWindow(
            wndClass.GetName(), name, style,
            CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
            NULL, NULL, wndClass.GetInstance(), this
        );

        if (hWnd == NULL) throw WND_LAST_ERROR();

        ShowWindow(hWnd, SW_SHOWDEFAULT);

        pGfx = std::make_unique<Graphics>(hWnd);
    }

    Window::~Window()
    {

        DestroyWindow(hWnd);
        hWnd = NULL;
    }

    Graphics& Window::Gfx()
    {
        if (!pGfx) throw WND_NO_GFX_EXCEPT();
        return *pGfx;
    }

    void Window::SetTitle(const std::string& title)
    {
        if (SetWindowText(hWnd, title.c_str()) == 0) throw WND_LAST_ERROR();
    }

    std::optional<int> Window::ProcessMessages()
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return (int)msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return { };
    }

    LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

            return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        switch (msg)
        {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;


        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (!(wParam & 0x40000000) || kbd.AutorepeatIsEnabled())
            {
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
            kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;
        case WM_CHAR:
            kbd.OnChar(static_cast<unsigned char>(wParam));
            break;
        case WM_KILLFOCUS:
            kbd.ClearState();
            break;

        case WM_MOUSEMOVE: {
            const POINTS pt = MAKEPOINTS(lParam);
            if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
            {
                mouse.OnMouseMove(pt.x, pt.y);
                if (!mouse.IsInWindow())
                {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }
            }
            else
            {
                if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
                {
                    mouse.OnMouseMove(pt.x, pt.y);
                }
                else
                {
                    mouse.OnMouseLeave();
                    ReleaseCapture();
                }
            }
            break; }
        case WM_LBUTTONUP: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftReleased(pt.x, pt.y);
            break; }
        case WM_LBUTTONDOWN: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftPressed(pt.x, pt.y);
            break; }
        case WM_RBUTTONUP: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightReleased(pt.x, pt.y);
            break; }
        case WM_RBUTTONDOWN: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightPressed(pt.x, pt.y);
            break; }
        case WM_MBUTTONUP: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnMiddleReleased(pt.x, pt.y);
            break; }
        case WM_MBUTTONDOWN: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnMiddlePressed(pt.x, pt.y);
            break; }
        case WM_MOUSEWHEEL: {
            const POINTS pt = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            mouse.OnWheelDelta(pt.x, pt.y, delta);
            break; }

                          //Hide system cursor
        case WM_SETCURSOR: {
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor(NULL);
                return true;
            }}
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }



    Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
        : Exception(line, file), hr(hr)
    {}

    const char* Window::HrException::what() const noexcept
    {
        std::stringstream oss;
        oss << GetType() << std::endl
            << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
            << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
            << "[Description] " << GetErrorString() << std::endl
            << GetOriginString();

        whatBuffer = oss.str();
        return whatBuffer.c_str();
    }

    const char* Window::HrException::GetType() const noexcept
    {
        return "Window Exception";
    }

    HRESULT Window::HrException::GetErrorCode() const noexcept
    {
        return hr;
    }

    std::string Window::HrException::GetErrorString() const noexcept
    {
        return TranslateMessage(hr);
    }

    const char* Window::NoGfxException::GetType() const noexcept
    {
        return "No Graphics Exception";
    }

    std::string Window::Exception::TranslateMessage(HRESULT hr)
    {
        char* pMsgBuf = NULL;
        const DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf),
            0, NULL);
        if (nMsgLen == 0)
        {
            return "Unidentified Error Code";
        }

        std::string errorString = pMsgBuf;
        LocalFree(pMsgBuf);
        return errorString;
    }
}