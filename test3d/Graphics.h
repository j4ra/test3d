#pragma once

#include "MiniWindows.h"
#include <d3d11.h>
#include "BaseException.h"

class Graphics
{
public:
    class Exception : public BaseException
    {
        using BaseException::BaseException;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
    private:
        HRESULT hr;
    };
    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;
    public:
        const char* GetType() const noexcept override;
    };

public:
    Graphics(HWND hWnd);
    Graphics& operator=(const Graphics&) = delete;
    Graphics(const Graphics&) = delete;
    ~Graphics();
    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept;
private:
    ID3D11Device* pDevice = NULL;
    IDXGISwapChain* pSwap = NULL;
    ID3D11DeviceContext* pContext = NULL;
    ID3D11RenderTargetView* pTarget = NULL;
};