#pragma once
#include "Time.h"
#include "Graphics.h"
#include <DirectXMath.h>

class GameObject;
using GameObjectPtr = std::unique_ptr<GameObject>;
class GameObject {
public:
    struct Input {
        const Application::Mouse& mouse;
        const Application::Keyboard& kbd;
    };
public:
    virtual ~GameObject() = default;
    inline virtual void Update(const Application::Time& time, Rendering::Graphics& gfx, const Input& input) {};
    inline virtual void Start(const Application::Time& time, Rendering::Graphics& gfx) {};

    inline DirectX::XMMATRIX GetTransofrm() {
        DirectX::XMMATRIX tr = transform;
        if (parent != nullptr) {
            tr = parent->GetTransofrm() * transform;
        }

        return tr;
    }

    inline void SetTransofrm(const DirectX::XMMATRIX& matrix) {
        transform = matrix;
    }

    inline void AddChild(GameObjectPtr&& child) {
        child->parent = this;
        children.emplace_back(std::move(child));
    }

private:
    DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
    std::vector<GameObjectPtr> children;
    GameObject* parent = nullptr;
};