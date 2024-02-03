#pragma once
#include "math-header.hpp"
#include <array>
#include <string>
namespace cc
{
    class AABB
    {
    public:
        AABB(float xs = -FLT_MAX, float xm = FLT_MAX, float ys = -FLT_MAX, float ym = FLT_MAX, float zs = -FLT_MAX, float zm = FLT_MAX);
        AABB(math::Vector3f const &_min, math::Vector3f const &_max);
        AABB(math::Vector3f const &a, math::Vector3f const &b, math::Vector3f const &c);
        AABB(math::Vector4f const &a, math::Vector4f const &b, math::Vector4f const &c);
        bool inside(math::Vector3f const &p) const;
        math::Vector3f get_center() const;
        std::array<math::Vector3f, 8> get_vertices();
        bool intersect(const AABB &box) const;
        bool inside(math::Vector3f const &a, math::Vector3f const &b, math::Vector3f const &c) const;
        std::string to_string() const { return std::to_string(x[0]) + " " + std::to_string(x[1]) + " " + std::to_string(y[0]) + " " + std::to_string(y[1]) + " " + std::to_string(z[0]) + " " + std::to_string(z[1]); }
        float x[2], y[2], z[2];
    };
}
