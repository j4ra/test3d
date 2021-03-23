#pragma once
#include "Graphics.h"

namespace Rendering {
    enum class CBUpdateType {
        Frame,
        Once
    };

    enum class CBBufferType {
        Vertex,
        Pixel
    };

    template<typename BufferStructure, CBBufferType t, CBUpdateType u>
    class ConstantBuffer {
    public:
        inline void Create(Graphics& gfx, const BufferStructure& data) {
            D3D11_BUFFER_DESC cbd;
            cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbd.Usage = D3D11_USAGE_DYNAMIC;
            cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbd.MiscFlags = 0u;
            cbd.ByteWidth = sizeof(BufferStructure);
            cbd.StructureByteStride = 0u;
            D3D11_SUBRESOURCE_DATA cbsd = {};
            cbsd.pSysMem = &data;

            gfx.CreateBuffer(cbd, cbsd, buffer_);
        }

        inline CBUpdateType GetUpdateType() const {
            return u;
        }

        inline void Update(Graphics& gfx, const BufferStructure& data) {
            D3D11_MAPPED_SUBRESOURCE msr;

            gfx.GetContext()->Map(buffer_.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
            memcpy(msr.pData, &data, sizeof(BufferStructure));
            gfx.GetContext()->Unmap(buffer_.Get(), 0u);
        }

        inline void Bind(Graphics& gfx, UINT slot) {
            if (t == CBBufferType::Vertex) {
                gfx.GetContext()->VSSetConstantBuffers(slot, 1u, buffer_.GetAddressOf());
            }
            else if (t == CBBufferType::Pixel) {
                gfx.GetContext()->PSSetConstantBuffers(slot, 1u, buffer_.GetAddressOf());
            }
        }
    private:
        Graphics::BufferPtr buffer_;
    };
}