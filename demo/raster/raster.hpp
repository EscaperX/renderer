#pragma once
#include <math/math-header.hpp>
#include <resource/model.hpp>

#include "vertex.hpp"
#include "simple-buffer.hpp"

#include <vector>

namespace cc
{
    class Raster
    {
    public:
        static auto instance() -> Raster &;
        auto initialize(ModelPtr ptr, math::Matrix4f const &view, math::Matrix4f const &project) -> void;
        auto clear_color(math::Vector3u8i const &v = {0, 0, 0}) -> void;
        auto clear_depth() -> void;
        auto set_viewport(int w, int h) -> void;
        auto render_target(Buffer2D<math::Vector3u8i> *c, Buffer2D<double> *d) -> void;
        // auto transform() -> void;
        // auto clip() -> void;
        auto rasterize() -> void;
        auto get_color() const -> math::Vector3u8i const *;
        auto get_depth() const -> double const *;

    private:
        Raster() = default;
        Raster(Raster const &) = delete;
        Raster &operator=(Raster const &) = delete;
        // scene
        ModelPtr model;

        // camera
        math::Matrix4f view;
        math::Matrix4f project;
        int viewport_height, viewport_width;

        // render target which can be replaced by framebuffer
        Buffer2D<math::Vector3u8i> color;
        Buffer2D<double> depth;

        // internal buffer for vertex after vertex shader
        std::vector<VertexOut> vertex_buffer;
    };
}