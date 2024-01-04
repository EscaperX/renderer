#pragma once

#include <math/math-header.hpp>

namespace cc
{
    /**
     * @brief Vertex that output from vertex shader (assume we have one)
     * There're several builtin parameters.
     */
    struct VertexOut
    {
        VertexOut(math::Vector3f const &p, math::Vector3f const &n, math::Vector2f const &tex, float _w) : position(p), normal(n), uv(tex), w(_w) {}
        math::Vector3f position;
        math::Vector3f normal;
        math::Vector2f uv;
        float w;
    };
}