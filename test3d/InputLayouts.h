#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <array>

namespace Rendering {
    namespace InputLayouts {
        namespace Internal {
            D3D11_INPUT_ELEMENT_DESC poscolLayout[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
        }

        struct InputLayoutInfo {
            D3D11_INPUT_ELEMENT_DESC* Layout;
            std::size_t Size;
        };

        enum Layout {
            PosCol,

            NumLayouts
        };

        std::array<InputLayoutInfo, NumLayouts> layouts = {
            { Internal::poscolLayout, 2 }
        };

        struct VertexFormatPosCol {
            float x;
            float y;
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;
            std::uint8_t a;
        };
    }
}