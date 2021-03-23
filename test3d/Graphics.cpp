#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = hrcall)) throw Graphics::HrException(__LINE__,__FILE__, hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)

namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

namespace Rendering {
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
        sd.BufferCount = 2;
        sd.OutputWindow = hWnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
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

    void Graphics::CreateVertexShader(const std::wstring& shaderSource,
        VertexShaderPtr& pVertexShader,
        InputLayoutPtr& pInputLayout,
        const D3D11_INPUT_ELEMENT_DESC* inputDesc,
        std::size_t inputDescSize)
    {
        HRESULT hr;
        WRL::ComPtr<ID3DBlob> pBlob;
        GFX_THROW_FAILED(D3DReadFileToBlob(shaderSource.c_str(), &pBlob));
        GFX_THROW_FAILED(pDevice->CreateVertexShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            NULL,
            &pVertexShader
        ));

        GFX_THROW_FAILED(pDevice->CreateInputLayout(
            inputDesc,
            (UINT)inputDescSize,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            &pInputLayout
        ));
    }

    void Graphics::CreatePixelShader(
        const std::wstring& shaderSource,
        PixelShaderPtr& pPixelShader
    )
    {
        HRESULT hr;
        WRL::ComPtr<ID3DBlob> pBlob;
        GFX_THROW_FAILED(D3DReadFileToBlob(shaderSource.c_str(), &pBlob));
        GFX_THROW_FAILED(pDevice->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            NULL,
            &pPixelShader
        ));
    }

    void Graphics::CreateBuffer(D3D11_BUFFER_DESC& bd, D3D11_SUBRESOURCE_DATA& sd, BufferPtr& buffer)
    {
        HRESULT hr;
        GFX_THROW_FAILED(pDevice->CreateBuffer(&bd, &sd, &buffer));
    }

    //void Graphics::DrawTestTriangle(float angle, float x, float y)
    //{
    //    WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    //
    //    struct Vertex 
    //    {
    //        float x;
    //        float y;
    //        BYTE r;
    //        BYTE g;
    //        BYTE b;
    //        BYTE a;
    //    };
    //
    //    const Vertex vertecies[] =
    //    {
    //        {0.0f, 0.5f, 255u, 0u, 0u, 255u},
    //        {0.5f, -0.5f, 0u, 255u, 0u, 255u},
    //        {-0.5f, -0.5f, 0u, 0u, 255u, 255u},
    //        {-0.3f, 0.3f, 0u, 255u, 0u, 255u},
    //        {0.3f, 0.3f, 0u, 0u, 255u, 255u},
    //        {0.0f, -0.8f, 255u, 0u, 0u, 255u}
    //        
    //        //{0.0f, 0.5f, 1.0f, 0.0f, 0.0f}
    //    };
    //
    //    D3D11_BUFFER_DESC bd = {};
    //    bd.Usage = D3D11_USAGE_DEFAULT;
    //    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //    bd.CPUAccessFlags = 0u;
    //    bd.MiscFlags = 0u;
    //    bd.ByteWidth = sizeof(vertecies);
    //    bd.StructureByteStride = sizeof(Vertex);
    //
    //    D3D11_SUBRESOURCE_DATA sd = {};
    //    sd.pSysMem = vertecies;
    //    
    //    HRESULT hr;
    //    GFX_THROW_FAILED(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
    //
    //    const unsigned short indicies[] =
    //    {
    //        0,1,2,
    //        0,2,3,
    //        0,4,1,
    //        2,1,5,
    //    };
    //
    //    WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
    //    D3D11_BUFFER_DESC ibd = {};
    //    ibd.Usage = D3D11_USAGE_DEFAULT;
    //    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //    ibd.CPUAccessFlags = 0u;
    //    ibd.MiscFlags = 0u;
    //    ibd.ByteWidth = sizeof(indicies);
    //    ibd.StructureByteStride = sizeof(unsigned short);
    //
    //    D3D11_SUBRESOURCE_DATA sdind = {};
    //    sdind.pSysMem = indicies;
    //
    //    GFX_THROW_FAILED(pDevice->CreateBuffer(&ibd, &sdind, &pIndexBuffer));
    //    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT , 0u);
    //
    //    struct ConstantBuffer
    //    {
    //        DX::XMMATRIX transform;
    //    };
    //
    //    const ConstantBuffer cb =
    //    {
    //        {
    //            DX::XMMatrixTranspose(
    //                DX::XMMatrixRotationZ(0.1f * angle * angle) *
    //                DX::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
    //                DX::XMMatrixTranslation(x, y, 0)
    //            )
    //        }
    //    };
    //    WRL::ComPtr<ID3D11Buffer> pConstBuffer;
    //    D3D11_BUFFER_DESC cbd;
    //    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    //    cbd.Usage = D3D11_USAGE_DYNAMIC;
    //    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //    cbd.MiscFlags = 0u;
    //    cbd.ByteWidth = sizeof(cb);
    //    cbd.StructureByteStride = 0u;
    //    D3D11_SUBRESOURCE_DATA cbsd = {};
    //    cbsd.pSysMem = &cb;
    //    
    //    GFX_THROW_FAILED(pDevice->CreateBuffer(&cbd, &cbsd, &pConstBuffer));
    //
    //    pContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
    //
    //    const UINT stride = sizeof(Vertex);
    //    const UINT offset = 0u;
    //
    //    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    //
    //    WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    //    WRL::ComPtr<ID3DBlob> pBlob;
    //    GFX_THROW_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
    //    GFX_THROW_FAILED(pDevice->CreateVertexShader(
    //        pBlob->GetBufferPointer(),
    //        pBlob->GetBufferSize(),
    //        NULL,
    //        &pVertexShader
    //        ));
    //
    //    pContext->VSSetShader(pVertexShader.Get(), NULL, 0);
    //
    //    WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    //    const D3D11_INPUT_ELEMENT_DESC ied[] =
    //    {
    //        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //        {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    //    };
    //
    //    GFX_THROW_FAILED(pDevice->CreateInputLayout(
    //        ied,
    //        (UINT)std::size(ied),
    //        pBlob->GetBufferPointer(),
    //        pBlob->GetBufferSize(),
    //        &pInputLayout
    //    ));
    //
    //    pContext->IASetInputLayout(pInputLayout.Get());
    //
    //    WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    //    GFX_THROW_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
    //    GFX_THROW_FAILED(pDevice->CreatePixelShader(
    //        pBlob->GetBufferPointer(),
    //        pBlob->GetBufferSize(),
    //        NULL,
    //        &pPixelShader
    //    ));
    //
    //    pContext->PSSetShader(pPixelShader.Get(), NULL, 0);
    //
    //    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), NULL);
    //
    //    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    //    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //
    //    D3D11_VIEWPORT vp;
    //    vp.Width = 800;
    //    vp.Height = 600;
    //    vp.MinDepth = 0;
    //    vp.MaxDepth = 1;
    //    vp.TopLeftX = 0;
    //    vp.TopLeftY = 0;
    //
    //    pContext->RSSetViewports(1u, &vp);
    //
    //    //pContext->Draw((UINT)std::size(vertecies), 0u);
    //    pContext->DrawIndexed((UINT)std::size(indicies), 0u, 0u);
    //}

    void Graphics::DrawIndexed(std::size_t size)
    {
        pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), NULL);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D11_VIEWPORT vp;
        vp.Width = 800;
        vp.Height = 600;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        pContext->RSSetViewports(1u, &vp);
        pContext->DrawIndexed((UINT)size, 0u, 0u);
    }




    Graphics::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
        :Exception(line, file), hr(hr) {}

    const char* Graphics::HrException::what() const noexcept
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

    const char* Graphics::HrException::GetType() const noexcept
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

    const char* Graphics::DeviceRemovedException::GetType() const noexcept
    {
        return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
    }
}