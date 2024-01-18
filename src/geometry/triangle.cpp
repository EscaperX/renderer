#include "triangle.hpp"

namespace cc
{
    auto sign(math::Vector3f const &p1, math::Vector3f const &p2, math::Vector3f const &p3) -> float
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }
    auto inside_triangle_2D(float x, float y, math::Vector3f const &v0, math::Vector3f const &v1, math::Vector3f const &v2) -> bool
    {
        float d1, d2, d3;
        bool has_neg, has_pos;
        math::Vector3f pt{x, y, 1.0f};
        d1 = sign(pt, v0, v1);
        d2 = sign(pt, v1, v2);
        d3 = sign(pt, v2, v0);

        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(has_neg && has_pos);
    }
    auto compute_barycentric_2D(float x, float y, math::Vector3f const &v0, math::Vector3f const &v1, math::Vector3f const &v2) -> std::tuple<float, float, float>
    {
        float c1 = (x * (v1.y - v2.y) + (v2.x - v1.x) * y + v1.x * v2.y - v2.x * v1.y) / (v0.x * (v1.y - v2.y) + (v2.x - v1.x) * v0.y + v1.x * v2.y - v2.x * v1.y);
        float c2 = (x * (v2.y - v0.y) + (v0.x - v2.x) * y + v2.x * v0.y - v0.x * v2.y) / (v1.x * (v2.y - v0.y) + (v0.x - v2.x) * v1.y + v2.x * v0.y - v0.x * v2.y);
        float c3 = (x * (v0.y - v1.y) + (v1.x - v0.x) * y + v0.x * v1.y - v1.x * v0.y) / (v2.x * (v0.y - v1.y) + (v1.x - v0.x) * v2.y + v0.x * v1.y - v1.x * v0.y);
        return {c1, c2, c3};
    }
} // namespace cc
