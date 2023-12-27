#pragma once

#include <math/math-header.hpp>
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
            // Todo: maybe use move semantics
            TriangleMesh(const std::vector<Vector3f> &ps, const std::vector<Vector3f> &ns,
                         const std::vector<Vector2f> &texs, const std::vector<size_t> &ids)
            {
                indices = ids;
                positions = ps;
                normals = ns;
                uvs = texs;
                indices = ids;
            }
            std::vector<size_t> indices;
            std::vector<Vector3f> positions;
            std::vector<Vector3f> normals;
            std::vector<Vector2f> uvs;
        };
    }
}
