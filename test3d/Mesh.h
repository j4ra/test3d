#pragma once

#include <vector>

class Mesh {
public:
    Mesh() {
        vertecies.push_back({ 0.0f, 0.5f, 255u, 0u, 0u, 255u });
        vertecies.push_back({ 0.5f, -0.5f, 0u, 255u, 0u, 255u });
        vertecies.push_back({ -0.5f, -0.5f, 0u, 0u, 255u, 255u });
        vertecies.push_back({ -0.3f, 0.3f, 0u, 255u, 0u, 255u });
        vertecies.push_back({ 0.3f, 0.3f, 0u, 0u, 255u, 255u });
        vertecies.push_back({ 0.0f, -0.8f, 255u, 0u, 0u, 255u });

        indices.push_back({ 0, 1, 2 });
        indices.push_back({ 0, 2, 3 });
        indices.push_back({ 0, 4, 1 });
        indices.push_back({ 2, 1, 5 });
    }

    inline virtual ~Mesh() = default;

    inline virtual std::size_t vb_size() const {
        return vertecies.size() * sizeof(Vertex);
    }

    inline virtual std::size_t vb_stride() const {
        return sizeof(Vertex);
    }

    inline virtual void* vb_data() const {
        return (void*)vertecies.data();
    }

    inline virtual std::size_t ib_size() const {
        return indices.size() * sizeof(Triangle);
    }

    inline virtual std::size_t ib_stride() const {
        return sizeof(short);
    }

    inline virtual void* ib_data() const {
        return (void*)indices.data();
    }

private:
    struct Vertex
    {
        float x;
        float y;
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
    };

    struct Triangle
    {
        short v0, v1, v2;
    };

    std::vector<Vertex> vertecies;

    std::vector<Triangle> indices;
};