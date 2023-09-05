#pragma once

#include "../math/math-header.hpp"

namespace nanami::util
{
    using math;

    struct Camera
    {
    public:
        Camera();

    private:
        Vector3f center;
        Vector3f dir;
        int width, height;
    };

}