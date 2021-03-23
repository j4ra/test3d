#pragma once

#include "MiniWindows.h"
#include <d3d11.h>
#include <wrl.h>
#include "BaseException.h"

namespace Rendering {
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
        using BufferPtr = Microsoft::WRL::ComPtr<ID3D11Buffer>;
        using VertexShaderPtr = Microsoft::WRL::ComPtr<ID3D11VertexShader>;
        using PixelShaderPtr = Microsoft::WRL::ComPtr<ID3D11PixelShader>;
        using InputLayoutPtr = Microsoft::WRL::ComPtr<ID3D11InputLayout>;
    public:
        Graphics(HWND hWnd);
        Graphics& operator=(const Graphics&) = delete;
        Graphics(const Graphics&) = delete;
        ~Graphics() = default;
        void EndFrame();
        void ClearBuffer(float r, float g, float b) noexcept;
        void CreateVertexShader(
            const std::wstring& shaderSource,
            VertexShaderPtr& pVertexShader,
            InputLayoutPtr& pInputLayout,
            const D3D11_INPUT_ELEMENT_DESC* inputDesc,
            std::size_t inputDescSize);
        void CreatePixelShader(
            const std::wstring& shaderSource,
            PixelShaderPtr& pPixelShader
        );
        inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return pContext; }
        void CreateBuffer(D3D11_BUFFER_DESC& bd, D3D11_SUBRESOURCE_DATA& sd, BufferPtr& buffer);
        void DrawIndexed(std::size_t size);
        //void DrawTestTriangle(float angle, float x, float y);
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
        Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
    };
}