#pragma once
#include "Graphics.h"
#include "ShaderProgram.h"
#include <d3dcompiler.h>
#include <vector>

namespace Rendering {
    class Material {
    public:
        inline void Bind(Graphics& gfx) {
            shader->Bind(gfx);
        }
    private:
        Shaders::ShaderProgram* shader = Shaders::ShaderBank::BasicShader.get();
    };
}