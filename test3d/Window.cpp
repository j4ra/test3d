#include "Window.h"
#include "resource.h"
#include <sstream>


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

    if (!RegisterClassEx(&wc)) throw BWND_LAST_ERROR();

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





Window::Window(int width, int height, const char* name) : wndClass(name)
{
    constexpr DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
    
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if (!AdjustWindowRect(&wr, style, FALSE)) throw BWND_LAST_ERROR();

    hWnd = CreateWindow(
        wndClass.GetName(), name, style,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, wndClass.GetInstance(), this
    );

    if (hWnd == NULL) throw BWND_LAST_ERROR();

    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(hWnd);
    hWnd = NULL;
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
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMouseMove(pt.x, pt.y);
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
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mouse.OnWheelUp(pt.x, pt.y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mouse.OnWheelDown(pt.x, pt.y);
        }
        break;
    }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}





Window::Exception::Exception(int line, const char * file, HRESULT hr) noexcept
    : BaseException(line, file), hr(hr)
{}

const char * Window::Exception::what() const noexcept
{
    std::stringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char * Window::Exception::GetType() const noexcept
{
    return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = NULL;
    DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
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

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr);
}
