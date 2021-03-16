#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include <DirectXMath.h>

class RenderComponent : public GameObject {
public:
    virtual ~RenderComponent() = default;
    RenderComponent() : m(), mat() {}

    inline void Update(const Application::Time& time, Graphics& gfx, const Input& input) override {
        float angle = time.TotalTime();
        float x = input.mouse.GetPosX() / 400.0f - 1.0f;
        float y = 1.0f - input.mouse.GetPosY() / 300.0f;
        
        DirectX::XMMATRIX matrix = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixRotationZ(0.1f * angle * angle) *
            DirectX::XMMatrixScaling(0.1, 0.1, 0.1) *
            DirectX::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
            DirectX::XMMatrixTranslation(x, y, 0)
        );
        SetTransofrm(
            matrix
        );

        Draw(gfx);
    };

    inline void Start(const Application::Time& time, Graphics& gfx) override {
        mat.Initialize(gfx);
        CreateBuffers(gfx);
    }

private:
    Mesh m;
    Material mat;
    Graphics::BufferPtr pIndexBuffer;
    Graphics::BufferPtr pVertexBuffer;
    Graphics::BufferPtr pConstantBuffer;
    struct TransformConstatnBuffer {
        DirectX::XMMATRIX transform;
    };
    TransformConstatnBuffer tcb = { GetTransofrm() };

    inline void Draw(Graphics& gfx) {
        tcb.transform = GetTransofrm();
        D3D11_MAPPED_SUBRESOURCE msr;

        gfx.GetContext()->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
        memcpy(msr.pData, &tcb, sizeof(TransformConstatnBuffer));
        gfx.GetContext()->Unmap(pConstantBuffer.Get(), 0u);

        gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
        gfx.GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
        const UINT stride = (UINT)m.vb_stride();
        const UINT offset = 0u;
        gfx.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

        mat.Bind(gfx);

        gfx.DrawIndexed(m.vb_size());
    }

    inline void CreateBuffers(Graphics& gfx) {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = (UINT)m.vb_size();
        bd.StructureByteStride = (UINT)m.vb_stride();

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = m.vb_data();

        gfx.CreateBuffer(bd, sd, pVertexBuffer);

        D3D11_BUFFER_DESC ibd = {};
        ibd.Usage = D3D11_USAGE_DEFAULT;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0u;
        ibd.MiscFlags = 0u;
        ibd.ByteWidth = (UINT)m.ib_size();
        ibd.StructureByteStride = (UINT)m.ib_stride();

        D3D11_SUBRESOURCE_DATA sdind = {};
        sdind.pSysMem = m.ib_data();

        gfx.CreateBuffer(ibd, sdind, pIndexBuffer);

        tcb.transform = GetTransofrm();
        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(tcb);
        cbd.StructureByteStride = 0u;
        D3D11_SUBRESOURCE_DATA cbsd = {};
        cbsd.pSysMem = &tcb;

        gfx.CreateBuffer(cbd, cbsd, pConstantBuffer);
    }
};