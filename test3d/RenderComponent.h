#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"

namespace Rendering {
    class RenderComponent : public GameObject {
    public:
        virtual ~RenderComponent() = default;
        RenderComponent() : m(), mat() {}

        inline void Update(const Application::Time& time, Graphics& gfx, const Input& input) override {
            float angle = time.TotalTime();
            float x = input.mouse.GetPosX() / 400.0f - 1.0f;
            float y = 1.0f - input.mouse.GetPosY() / 300.0f;

            SetTransofrm(
                DirectX::XMMatrixTranspose(
                    DirectX::XMMatrixRotationZ(0.1f * angle * angle) *
                    DirectX::XMMatrixScaling(0.1, 0.1, 0.1) *
                    DirectX::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
                    DirectX::XMMatrixTranslation(x, y, 0)
                )
            );

            Draw(gfx);
        };

        inline void Start(const Application::Time& time, Graphics& gfx) override {
            CreateBuffers(gfx);
        }

    private:
        Mesh<> m;
        Material mat;
        Graphics::BufferPtr pIndexBuffer;
        Graphics::BufferPtr pVertexBuffer;
        struct TransformConstatnBuffer {
            DirectX::XMMATRIX transform;
        };
        ConstantBuffer<TransformConstatnBuffer, CBBufferType::Vertex, CBUpdateType::Frame> transformBuffer;
        TransformConstatnBuffer tcb = { GetTransofrm() };

        inline void Draw(Graphics& gfx) {
            tcb.transform = GetTransofrm();
            
            transformBuffer.Update(gfx, tcb);          
            transformBuffer.Bind(gfx, 0u);

            gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
            
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

            transformBuffer.Create(gfx, tcb);
        }
    };
}