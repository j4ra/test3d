#pragma once
#include "Time.h"
#include "Graphics.h"
#include <DirectXMath.h>



class GameObject {
public:
    struct Input {
        const Mouse& mouse;
        const Keyboard& kbd;
    };
public:
    virtual ~GameObject() = default;
    inline virtual void Update(const Application::Time& time, Graphics& gfx, const Input& input) {};
    inline virtual void Start(const Application::Time& time, Graphics& gfx) {};

    inline DirectX::XMMATRIX GetTransofrm() {
        return transform;
    }

    inline void SetTransofrm(DirectX::XMMATRIX& matrix) {
        transform = matrix;
    }

private:
    DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
};