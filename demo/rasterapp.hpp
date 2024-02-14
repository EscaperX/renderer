#pragma once
#include "simple-buffer.hpp"

#include <gl/application.hpp>
#include <glad/glad.h>

#include <memory>
#include <resource/model.hpp>
#include <resource/camera.hpp>
#include <accelerator/octree.hpp>
namespace cc
{
    class RasterApp : public Application
    {
    public:
        RasterApp();
        ~RasterApp();
        auto update() -> void override;
        auto render() -> void override;
        auto init() -> void override;

    private:
        GLuint texture; // Final display texture sent to imgui
        std::vector<ModelPtr> models;
        std::unique_ptr<OctreeNode> scene_octree;
        Camera camera;
    };
};