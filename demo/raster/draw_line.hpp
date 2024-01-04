#pragma once
#include <math/math-header.hpp>
#include "../simple-buffer.hpp"

namespace cc
{
    // Bresenham's line drawing algorithm
    void draw_line(math::Vector3i begin, math::Vector3i end, Buffer2D<math::Vector3u8i> &color);
}