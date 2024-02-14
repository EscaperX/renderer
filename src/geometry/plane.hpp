#pragma once
#include <math/math-header.hpp>

namespace cc
{
    struct Plane final
    {
        Plane() {}
        Plane(math::Vector3f const &point, math::Vector3f const &n) //: normal(glm::normalize(n)), dist(glm::dot(point, normal)) {}
        {
            normal = glm::normalize(n);
            dist = glm::dot(point, normal);
        }
        auto signed_distance(math::Vector3f const &point) const -> float { return glm::dot(normal, point) - dist; }
        float dist;
        math::Vector3f normal;
    };
} // namespace cc
