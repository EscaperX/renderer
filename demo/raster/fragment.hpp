#pragma once
#include <math/math-header.hpp>

namespace cc
{
    /**
     * @brief Interpolated attribute for material
     *
     */
    struct Fragment
    {
        math::Vector2i fragcoord;
        // math::Vector4f position;
        float z;
        math::Vector3f normal;
        math::Vector2f uv;
    };

} // namespace cc
