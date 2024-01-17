#pragma once
#include <math/math-header.hpp>
#include <resource/model.hpp>
namespace cc
{
    class Raster
    {
    public:
    private:
        ModelPtr model;
        math::Matrix4f view;
        math::Matrix4f project;
    };
}