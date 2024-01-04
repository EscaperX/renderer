#pragma once
#include <vector>
#include <math/math-header.hpp>

namespace cc
{
    struct Mesh
    {
        std::vector<math::Vector3f> positions;
        std::vector<math::Vector3f> normals;
        std::vector<math::Vector2f> uvs;
        std::vector<uint32_t> indices;
    };
}