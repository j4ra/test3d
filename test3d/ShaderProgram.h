#pragma once
#include <memory>
#include "InputLayouts.h"
#include "Graphics.h"

namespace Rendering {
    namespace Shaders {
        class ShaderProgram {
        public:
            using ShaderProgramPtr = std::unique_ptr<ShaderProgram>;
        public:
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram& operator=(const ShaderProgram&) = delete;

            inline ShaderProgram(const std::wstring& vs, const std::wstring& ps, Rendering::InputLayouts::Layout l) {
                vs_source = vs;
                ps_source = ps;
                input_layout = Rendering::InputLayouts::layouts[l];
            }

            inline void Create(Graphics& gfx) {
                if (!init_) {
                    init_ = true;
                    gfx.CreateVertexShader(vs_source, vs_, il_, input_layout.Layout, input_layout.Size);
                    gfx.CreatePixelShader(ps_source, ps_);
                }
            }

            inline void Bind(Graphics& gfx) {
                gfx.GetContext()->VSSetShader(vs_.Get(), NULL, 0);
                gfx.GetContext()->IASetInputLayout(il_.Get());

                gfx.GetContext()->PSSetShader(ps_.Get(), NULL, 0);
            }
        private:
            std::wstring vs_source, ps_source;
            Rendering::InputLayouts::InputLayoutInfo input_layout;
            bool init_ = false;

            Graphics::VertexShaderPtr vs_;
            Graphics::PixelShaderPtr ps_;
            Graphics::InputLayoutPtr il_;
        };

        namespace ShaderBank {
            ShaderProgram::ShaderProgramPtr BasicShader = std::make_unique<ShaderProgram>(L"VertexShader.cso", L"PixelShader.cso", Rendering::InputLayouts::PosCol);
        
            inline void InitializeShaders(Graphics& gfx) {
                BasicShader->Create(gfx);
            }
        }
    }
}