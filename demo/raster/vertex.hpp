#pragma once

#include <math/math-header.hpp>
#include <span>

namespace cc
{

    /**
     * @brief Pending Draw data after occlusion culling at CPU side
     *
     */
    struct DrawCallBuffer
    {
        std::span<math::Vector3f> positions;
        std::span<math::Vector3f> normals;
        std::span<math::Vector2f> uvs;
        std::span<uint32_t> indices;
    };

    /**
     * @brief Vertex that output from vertex shader (assume we have one)
     * There're several builtin parameters.
     */
    struct Vertex
    {
        Vertex(math::Vector4f const &p, math::Vector3f const &n, math::Vector2f const &tex) : position(p), normal(n), uv(tex) {}
        math::Vector4f position;
        math::Vector3f normal;
        math::Vector2f uv;
    };
}