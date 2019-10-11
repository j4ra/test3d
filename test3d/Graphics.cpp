#include "Graphics.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = hrcall)) throw Graphics::HrException(__LINE__,__FILE__, hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)

namespace WRL = Microsoft::WRL;

Graphics::Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = { };

    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    UINT deviceFlags = 0;
#ifndef NDEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    HRESULT hr;

    GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        deviceFlags,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        NULL,
        &pContext
    ));

    WRL::ComPtr<ID3D11Resource> pBackBuffer;
    GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pTarget));
}

void Graphics::EndFrame()
{
    HRESULT hr;

    if (FAILED(hr = pSwap->Present(1u, 0)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            GFX_THROW_FAILED(hr);
        }
    }
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = { r, g, b, 1.0 };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
}






Graphics::HrException::HrException(int line, const char * file, HRESULT hr) noexcept
    :Exception(line, file), hr(hr) {}

const char * Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Description]" << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char * Graphics::HrException::GetType() const noexcept
{
    return "Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
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

const char * Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
