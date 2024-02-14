#pragma once

#include <vector>

#include <geometry/AABB.h>
#include "../simple-buffer.hpp"

namespace cc
{
    class HieZbuffer
    {
    public:
        HieZbuffer(int width, int height);
        ~HieZbuffer();
        // bool is_hidden(math::Vector3f const &v0, math::Vector3f const &v1, math::Vector3f const &v2);
        auto is_hidden(const AABB &box) const -> bool;
        auto initialize(Buffer2D<float> const *depth) -> void;
        auto update(int w, int h, float z) -> void;
        auto get_z(int w, int h) const -> float;
        // void clear();

    private:
        std::vector<Buffer2D<float>> zbuffers;
        AABB screen_box;
        // extend to 2^i for both width and height
        int buffer_width, buffer_height;
    };
} // namespace cc
