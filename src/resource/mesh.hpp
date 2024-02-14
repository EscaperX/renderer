#pragma once
#include <vector>
#include <math/math-header.hpp>
#include <geometry/AABB.h>
namespace cc
{
    struct Mesh
    {
        AABB bbx;
        std::vector<math::Vector3f> positions;
        std::vector<math::Vector3f> normals;
        std::vector<math::Vector2f> uvs;
        std::vector<uint32_t> indices;
    };
}