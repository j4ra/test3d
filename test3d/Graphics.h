#pragma once

#include "MiniWindows.h"
#include <d3d11.h>

class Graphics
{
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