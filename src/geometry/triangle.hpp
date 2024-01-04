#pragma once
#include <math/math-header.hpp>
#include <tuple>

namespace cc
{
    auto inside_triangle_2D(float x, float y, math::Vector3f const &v0, math::Vector3f const &v1, math::Vector3f const &v2) -> bool;
    auto compute_barycentric_2D(float x, float y, math::Vector3f const &v0, math::Vector3f const &v1, math::Vector3f const &v2) -> std::tuple<float, float, float>;
} // namespace cc
