#pragma once
#include "math-header.hpp"
#include <vector>

namespace nanami
{
    namespace geometry
    {
        using namespace math;
        struct Vertex
        {
            Vector3f pos;
            Vector3f normals;
            Vector2f uv;
        };
        struct Triangle
        {
            Vertex a, b, c;
        };
        struct Shpere
        {
            float radius;
            Vector3f center;
        };

        struct TriangleMesh
        {
            std::vector<size_t> indices;
            std::vector<Vector3f> positions;
            std::vector<Vector3f> normals;
            std::vector<Vector2f> uvs;
        };
    }
}
