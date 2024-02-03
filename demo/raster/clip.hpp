#pragma once

#include "vertex.hpp"

#include <vector>

namespace cc
{
    auto inside_frustum(Vertex const &v, float near, float far) -> bool;
    auto frustum_clipping(Vertex const &v0, Vertex const &v1, Vertex const &v2) -> std::vector<Vertex>;
} // namespace cc
