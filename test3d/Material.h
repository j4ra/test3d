#pragma once
#include "Graphics.h"
#include <d3dcompiler.h>
#include <vector>

class Material {
public:
    inline Material() {
        ied.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        ied.push_back({ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }

    inline void Initialize(Graphics& gfx) {
        gfx.CreateVertexShader(L"VertexShader.cso", vertexShader, inputLayout, ied.data(), ied.size());
        gfx.CreatePixelShader(L"PixelShader.cso", pixelShader);
    }

    inline void Bind(Graphics& gfx) {
        gfx.GetContext()->VSSetShader(vertexShader.Get(), NULL, 0);
        gfx.GetContext()->IASetInputLayout(inputLayout.Get());

        gfx.GetContext()->PSSetShader(pixelShader.Get(), NULL, 0);
    }

private:
    Graphics::VertexShaderPtr vertexShader;
    Graphics::InputLayoutPtr inputLayout;

    Graphics::PixelShaderPtr pixelShader;

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
};